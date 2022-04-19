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

        // if consumer meets production limit
        if(upc->broker->requestsConsumed >= upc->broker->productionLimit)
        {
            // allow termination of main thread
            sem_post(&upc->broker->precedence);

            // terminate current thread
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
