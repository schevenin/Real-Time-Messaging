#include "producer.h"

void *produce(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    // item to be added to queue
    int item = upc->type;
    int index = 0;

    while (true) 
    {
        // sleep for time to produce
        usleep(upc->sleepTime);

        // if producing HDR, wait for HDR slot
        if (item == HumanDriver)
        {
            sem_wait(&upc->broker->emptyHumanSlots);
        }

        // wait for empty slot in queue
        sem_wait(&upc->broker->emptySlots);

        // obtain exclusive critical section access
        sem_wait(&upc->broker->mutex);

        // if producer meets production limit
        if(upc->broker->requestsProduced >= upc->broker->productionLimit)
        {
            // release exclusive critical section access
            sem_post(&upc->broker->mutex);

            // end thread
            break;
        }

        // add to queue
        upc->broker->buffer.push(item);

        // update producer counters
        upc->broker->requestsProduced += 1;
        upc->broker->inRequestQueue[item] += 1;
        upc->broker->produced[item] += 1;

        // output
        io_add_type((Requests) item, upc->broker->inRequestQueue, upc->broker->produced);

        // release exclusive critical section access
        sem_post(&upc->broker->mutex);
        
        // inform consumer there are filled slots
        sem_post(&upc->broker->filledSlots);
    }

    return (void *) NULL;
}
