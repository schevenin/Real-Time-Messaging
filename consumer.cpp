#include "consumer.h"

void *consume(void *ptr)
{
    UniquePC *upc = (UniquePC *)ptr;

    int item = upc->type;
    int index = 0;
    int removed;
    int value;

    while (true)
    {
        // wait for filled slots
        sem_wait(&upc->broker->filledSlots);


        // access buffer exclusively
        sem_wait(&upc->broker->mutex);

        sem_getvalue(&upc->broker->filledSlots, &value);
        printf("%d\n",value);

        // remove item from buffer
        upc->broker->buffer.pop();
        index = (index+1) % BUFFER_CAP;


        // update consumed counter
        upc->broker->requestsConsumed += 1;     
        
        //std::cout << "Requests produced: " << upc->broker->requestsProduced << std::endl;
        printf("Consumes produced: %i\n", upc->broker->requestsConsumed);
        //fflush(stdout);

        if(upc->broker->requestsConsumed>=upc->broker->productionLimit){
            sem_post(&upc->broker->precedence);
            break;
        }

        // release exclusive access to buffer
        sem_post(&upc->broker->mutex);

        // inform producer there are empty slots
        sem_post(&upc->broker->emptySlots);

        // sleep for time to consume
        sleep(upc->sleepTime);

    }

}
