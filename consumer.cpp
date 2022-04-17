#include "consumer.h"

void *consume(void *ptr)
{
    Monitor *monitor = (Monitor *)ptr;

    while (true) 
    {
        // 
        sem_wait(&monitor->filledSlots);

        // access buffer exclusively
        monitor->buffer.pop();



    }

}
