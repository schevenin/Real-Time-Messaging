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

        // place new item in buffer
        upc->broker->buffer.push(item);

        // release exclusive access to buffer
        sem_post(&upc->broker->mutex);
        
        // inform consumer there are filled slots
        sem_post(&upc->broker->filledSlots);

        // obtain access to number of requests produced
        sem_wait(&upc->broker->production);

        // update produced counter
        upc->broker->requestsProduced += 1; 

        printf(" (+) Request produced: %i\n", upc->broker->requestsProduced);
        
        // when producer meets production limit
        if(upc->broker->requestsProduced >= upc->broker->productionLimit)
        {
            printf(" (*) DONE PRODUCING.\n");
            break;
        }

        // release access to number of requests produced
        sem_post(&upc->broker->production);
    }

    return (void *) NULL;
}
