#include "consumer.h"

void *consume(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    int item = upc->type;
    int index = 0;


    while (upc->broker->requestsConsumed<upc->broker->productionLimit-1)
    {
        // wait for filled slots
        sem_wait(&upc->broker->filledSlots);

        // access buffer exclusively
        sem_wait(&upc->broker->mutex);

        // remove item from buffer
        int removed = upc->broker->buffer[index];
        index = (index+1) % BUFFER_CAP;

        // update consumed counter
        upc->broker->requestsConsumed += 1;     
        
        // printing
        //std::cout << "Requests consumed: " << upc->broker->requestsConsumed << std::endl;
        printf("Requests consumed: %i\n", upc->broker->requestsConsumed);
        //fflush(stdout);
 
        // release exclusive access to buffer
        sem_post(&upc->broker->mutex);

        // inform producer there are empty slots
        sem_post(&upc->broker->emptySlots);

        // sleep for time to consume
        sleep(upc->sleepTime);

    }

}
