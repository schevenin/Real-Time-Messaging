#include "producer.h"

void *produce(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    // item to be added to queue
    int item = upc->type;
    int index = 0;

    while (true) 
    {
        // sleep for production time
        sleep(upc->sleepTime);

        // wait for empty slots
        sem_wait(&upc->broker->emptySlots);

        // access buffer exclusively
        sem_wait(&upc->broker->mutex);

        // place new item in buffer
        upc->broker->buffer[index] = item;
        index = (index+1) % BUFFER_CAP;

        // update produced counter
        upc->broker->requestsProduced += 1;     
        
        // printing
        //std::cout << "Requests produced: " << upc->broker->requestsProduced << std::endl;
        printf("Requests produced: %i\n", upc->broker->requestsProduced);
        //fflush(stdout);

        // release exclusive access to buffer
        sem_post(&upc->broker->mutex);
        
        // inform consumer there are filled slots
        sem_post(&upc->broker->filledSlots);
    }
}
