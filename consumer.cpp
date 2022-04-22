/**
 * @file consumer.cpp
 * @authors Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief consumer function definition
 * @date 2022-04-21
 */

#include "consumer.h"

/**
 * @brief Consumer execution
 *
 * @param ptr pointer to PC object containing shared data structure
 */
void *consume(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr; // PC object

    // type of algorithm for consumption
    int algorithmType = upc->type;

    // type of item being consumed
    int item; 
    
    // consume
    while (true)
    {
        // no more requests will be produced and buffer is empty
        if ((upc->broker->requestsProduced == upc->broker->productionLimit) && (upc->broker->buffer.size() == 0))
        {
            printf("Consumer %i has no more requests to consume, consumer leaving.\n", algorithmType);

            // signal to main thread that a consumer finished
            sem_post(&upc->broker->precedence);

            // terminate current consumer thread
            break;
        }

        printf("Consumer %i waiting on filled slots.\n", algorithmType);

        // wait for filled slots
        sem_wait(&upc->broker->filledSlots);            

        printf("Consumer %i waiting on mutex access.\n", algorithmType);

        // obtain exclusive critical section access
        sem_wait(&upc->broker->mutex);

        // remove from queue and save item
        item = upc->broker->buffer.front();
        upc->broker->buffer.pop();

        upc->broker->inRequestQueue[item] -= 1;
        
        // release exclusive critical section access
        sem_post(&upc->broker->mutex);

        // inform producer there are empty slots
        sem_post(&upc->broker->emptySlots);

        // if item consumed was HDR, inform producer there is space for HDR
        if (item == HumanDriver)
        {
            sem_post(&upc->broker->emptyHumanSlots);
        }

        // sleep for time to consume
        usleep(upc->sleepTime);

        upc->broker->requestsConsumed += 1;
        upc->broker->consumed[algorithmType][item] += 1;

        // output
        //io_remove_type((Consumers) algorithmType, (Requests) item, upc->broker->inRequestQueue, upc->broker->consumed[algorithmType]);
        printf("Consumer %i finished consuming request. Total: %i\n", algorithmType, upc->broker->requestsConsumed);

        // if all requests have been consumed and buffer is empty
        if ((upc->broker->requestsConsumed == upc->broker->productionLimit) && (upc->broker->buffer.size() == 0))  {
            
            // signal to main thread that a consumption reached production limit
            sem_post(&upc->broker->precedence);
            sem_post(&upc->broker->precedence);

            break;
        }
        
    }

    return (void *) NULL;
}
