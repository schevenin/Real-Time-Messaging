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

        printf("Consumer type %i: waiting for filled slots\n", algorithmType);

        // wait for filled slots
        sem_wait(&upc->broker->filledSlots);

        printf("Consumer type %i: waiting for access to critical section\n", algorithmType);

        // obtain exclusive critical section access
        sem_wait(&upc->broker->mutex);
        
        printf("Consumer type %i: in critical section\n", algorithmType);

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

        // release exclusive critical section access
        sem_post(&upc->broker->mutex);

        // inform producer there are empty slots
        sem_post(&upc->broker->emptySlots);

        // started sleeping
        // sem_wait(&upc->broker->sleeping);
        // sleep for time to consume
        usleep(upc->sleepTime);
        // finished sleeping
        //sem_post(&upc->broker->sleeping);


        printf("Requests consumed: %i\n", upc->broker->requestsConsumed);
        printf("Buffer empty: %i\n", upc->broker->buffer.size() == 0);

        // if consumer meets production limit & queue is empty
        if ((upc->broker->requestsConsumed >= upc->broker->productionLimit) && (upc->broker->buffer.size() == 0))
        {            

            printf("CONSUMER FINISHED.\n");

            // sem_wait(&upc->broker->sleeping);
            // sem_wait(&upc->broker->sleeping);

            // signal main thread that a consumer finished
            sem_post(&upc->broker->precedence);

            // terminate current consumer thread
            break;
        }
    }

    return (void *) NULL;
}
