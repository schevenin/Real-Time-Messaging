#include "customer.h"

void *consume(void *ptr){
    Monitor *sem = (Monitor *)ptr;
     std::cout<<sem->consumerType;
    if(sem->consumerType==0){
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
