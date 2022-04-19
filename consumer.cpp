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

        // update consumed counter 
        upc->broker->requestsConsumed += 1; 

        // add Type
        if (algorithmType == CostAlgoDispatch)
        {
            upc->broker->consumed[CostAlgoDispatch] += 1;
        }
        else if (algorithmType == FastAlgoDispatch)
        {
            upc->broker->consumed[FastAlgoDispatch] += 1;
        }

        // remove from queue
        if (item == RoboDriver)
        {
            upc->broker->inRequestQueue[RoboDriver] -= 1;
        }
        else if (item == HumanDriver)
        {
            upc->broker->inRequestQueue[HumanDriver] -= 1;
        }

        // output
        //printf(" (-) Request consumed: %i\n", upc->broker->requestsConsumed);
        io_remove_type((Consumers) algorithmType, (Requests) item, upc->broker->inRequestQueue, upc->broker->consumed);

        // if consumer meets production limit
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
        sleep(upc->sleepTime);

    }
    return (void *) NULL;
}
