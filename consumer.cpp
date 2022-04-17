#include "consumer.h"

void *consume(void *ptr)
{
    Monitor *monitor = (Monitor *)ptr;

    while (true) 
    {
        // down the filledSlots semaphore
        sem_wait(&monitor->filledSlots);

        // access buffer exclusively
        monitor->buffer.pop();

        

    }

}
