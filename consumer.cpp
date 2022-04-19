#include "consumer.h"

void *consume(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    int algorithmType = upc->type;
    int index = 0;
    int item;
    int removed;
    int value;

    while (true)
    {
        // wait for filled slots
        sem_wait(&upc->broker->filledSlots);

        // obtain exclusive critical section access
        sem_wait(&upc->broker->mutex);
        
        // remove from queue and save item
        item = upc->broker->buffer.front();
        upc->broker->buffer.pop();

        // if item consumed was HDR, inform producer there is space for HDR
        if (item == HumanDriver)
        {
            sem_post(&upc->broker->emptyHumanSlots);
        }

        // update consumer counters
        upc->broker->requestsConsumed += 1; 
        upc->broker->consumed[algorithmType][item] += 1;
        upc->broker->inRequestQueue[item] -= 1;

        // output
        io_remove_type((Consumers) algorithmType, (Requests) item, upc->broker->inRequestQueue, upc->broker->consumed[algorithmType]);

        // if consumer meets production limit, allow termination of main thread
        if(upc->broker->requestsConsumed >= upc->broker->productionLimit)
        {
            sem_post(&upc->broker->precedence);
            break;
        }

        // release exclusive critical section access
        sem_post(&upc->broker->mutex);

        // inform producer there are empty slots
        sem_post(&upc->broker->emptySlots);

        // sleep for time to consume
        usleep(upc->sleepTime);
    }

    return (void *) NULL;
}
