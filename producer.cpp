#include "producer.h"

void *produce(void *ptr)
{
    Monitor *monitor = (Monitor *)ptr;

    int item = 0;

    while (monitor->requestsProduced != monitor->productionLimit) 
    {
        // sleep for production time
        sleep(monitor->autoDriverProductionTime);

        // if buffer is full, down the emptySlots semaphore
        if (monitor->buffer.size() == monitor->bufferCapacity)
        {
            sem_wait(&monitor->emptySlots);
        }

        // if item to be added will be the only item in buffer
        bool onlyItem = (monitor->buffer.size() == 0);
        
        // place new item in buffer
        monitor->buffer.push(item);
        monitor->filledSlots += 1;

        // if first item in buffer, up filledSlots semaphore
        if (onlyItem)
        {
            sem_post(&monitor->filledSlots);
        }

        
        monitor->requestsProduced += 1;
    }
}
