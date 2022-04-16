#include "producer.h"

void *produce(void *ptr)
{
    Monitor *monitor = (Monitor *)ptr;

    int request = 0;

    while (monitor->requestsProduced != monitor->productionLimit) 
    {
        while (monitor->buffer.size() == monitor->bufferCapacity)
        {
            sem_wait(monitor->emptySlots);
        }

        bool onlyItem = (monitor->buffer.size() == 0);
        monitor->buffer.push(request);
        
        if (onlyItem)
        {
            sem_post(monitor->filledSlots);
        }

        sleep(monitor->humanDriverProductionTime);
    }




    
}
