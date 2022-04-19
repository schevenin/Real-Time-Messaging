#include "producer.h"

void *produce(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    // item to be added to queue
    int item = upc->type;
    int index = 0;

    while (true) 
    {
        // sleep for production time
        sleep(upc->sleepTime);

        // wait for empty slots
        sem_wait(&upc->broker->emptySlots);

        // access buffer exclusively
        sem_wait(&upc->broker->mutex);

        // when producer meets production limit
        if(upc->broker->requestsProduced >= upc->broker->productionLimit)
        {
            // release exclusive access to buffer
            sem_post(&upc->broker->mutex);
            
            // inform consumer there are filled slots
            sem_post(&upc->broker->filledSlots);

            // end thread
            break;
        }

        // update produced counter
        upc->broker->requestsProduced += 1; 

        // add to queue
        if(item == RoboDriver)
        {
            upc->broker->inRequestQueue[HumanDriver] += 1;
            upc->broker->produced[RoboDriver] += 1;
        }
        else if (item == HumanDriver)
        {
            upc->broker->inRequestQueue[RoboDriver] += 1;
            upc->broker->produced[HumanDriver] += 1;
        }

        // output
        //printf(" (+) Request produced: %i\n", upc->broker->requestsProduced);
        io_add_type((Requests) item, upc->broker->inRequestQueue, upc->broker->produced);

        // release exclusive access to buffer
        sem_post(&upc->broker->mutex);
        
        // inform consumer there are filled slots
        sem_post(&upc->broker->filledSlots);
    }

    return (void *) NULL;
}
