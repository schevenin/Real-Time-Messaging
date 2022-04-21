/**
 * @file producer.cpp
 * @authors Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief producer function definition
 * @date 2022-04-21
 */

#include "producer.h"
#include "io.h"

/**
 * @brief Producer execution
 *
 * @param ptr pointer to shared data structure
 */
void *produce(void *ptr)
{
    // PC object
    UniquePC *upc = (UniquePC *)ptr; 

    // item to be added to queue
    int item = upc->type;

    // produce
    while (true) 
    {

        // if producer meets production limit
        if(upc->broker->requestsProduced >= upc->broker->productionLimit)
        {

            printf("Requests produced met limit, killing current thread.\n");

            // release exclusive critical section access
            sem_post(&upc->broker->mutex);

            // end thread
            break;
        }

        // sleep for time to produce
        usleep(upc->sleepTime);

        printf("Producer %i produced request. Total: %i\n", item, upc->broker->requestsProduced);

        // if producing HDR, wait for HDR slot
        if (item == HumanDriver)
        {
            sem_wait(&upc->broker->emptyHumanSlots);
        }

        printf("Producer %i waiting for empty slots.\n", item);

        // wait for empty slot in queue
        sem_wait(&upc->broker->emptySlots);

        printf("Producer %i waiting for mutex access.\n", item);

        // obtain exclusive critical section access
        sem_wait(&upc->broker->mutex);

        printf("Producer %i obtained mutex access.\n", item);

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

        printf("Producer %i released mutex access.\n", item);
        
        // inform consumer there are filled slots
        sem_post(&upc->broker->filledSlots);

        printf("Producer %i informed consumer of filled slots.\n", item);
    }

    return (void *) NULL;
}
