#include "consumer.h"

void *consume(void *ptr)
{
    Broker *broker = (Broker *)ptr;

    while (true) 
    {
        // down the filledSlots semaphore
        sem_wait(&broker->filledSlots);

        // access buffer exclusively
        broker->buffer.pop();

        

    }

}
