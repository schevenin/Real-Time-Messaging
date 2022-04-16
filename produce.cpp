#include "produce.h"
void *produce(void *ptr){
    Monitor *sem = (Monitor *)ptr;
     std::cout<<sem->consumerType;
    if(sem->requestType==0){
     printf("Hello, world!\n");
     sleep(1);
      printf(" world!\n");
    }
    else{
        printf(" world!\n");
        sleep(1);
        printf(" Hello,      world!\n");
    }
}
