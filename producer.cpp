#include "producer.h"

void *produce(void *ptr)
{
    Broker *broker = (Broker *)ptr;

    int item = 0;

    while (broker->requestsProduced != broker->productionLimit) 
    {
        // sleep for production time
        sleep(broker->autoDriverProductionTime);

        // if buffer is full, down the emptySlots semaphore
        if (broker->buffer.size() == broker->bufferCapacity)
        {
            sem_wait(&broker->emptySlots);
        }

        // if item to be added will be the only item in buffer
        bool onlyItem = (broker->buffer.size() == 0);
        
        // place new item in buffer
        broker->buffer.push(item);
        broker->filledSlots += 1;

        // if first item in buffer, up filledSlots semaphore
        if (onlyItem)
        {
            sem_post(&broker->filledSlots);
        }

        
        broker->requestsProduced += 1;
    }
}
