#include "consumer.h"

void *consume(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    int item = upc->type;
    int index = 0;
    int removed;
    int value;

    while (true)
    {
        // wait for filled slots
        sem_wait(&upc->broker->filledSlots);

        // access buffer exclusively
        sem_wait(&upc->broker->mutex);

        // remove item from buffer
        upc->broker->buffer.pop();

        // testing
        sem_getvalue(&upc->broker->filledSlots, &value);
        //printf("filled slots: %d\n",value);

        // release exclusive access to buffer
        sem_post(&upc->broker->mutex);

        // inform producer there are empty slots
        sem_post(&upc->broker->emptySlots);

        // sleep for time to consume
        sleep(upc->sleepTime);

        // obtain access to number of requests consumed
        sem_wait(&upc->broker->consumption);

        // update consumed counter 
        upc->broker->requestsConsumed += 1;    

        // printing
        printf(" (-) Request consumed: %i\n", upc->broker->requestsConsumed);

        // when consumer meets production limit
        if(upc->broker->requestsConsumed >= upc->broker->productionLimit)
        {
            printf(" (*) DONE CONSUMING.\n");
            sem_post(&upc->broker->precedence);
            break;
        }

        // release access to number of requests consumed
        sem_post(&upc->broker->consumption);

    }
    return (void *) NULL;
}
