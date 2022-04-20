/**
 * @file rideshare.cpp
 * @authors Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Real-time Messaging for Ridesharing entry point
 * @date 2022-04-21
 */

#include "producer.h"
#include "consumer.h"

/**
 * @brief Main Execution of Real-time Messaging for Ridesharing
 *
 * @param argc integer amount of arguments
 * @param argv character array of arguments
 * @return int value indicating exit status
 */
int main(int argc, char **argv)
{
    // initialize shared data structure
    Broker *broker = new Broker();
    broker->produced = new int[RequestTypeN];
    broker->consumed = new int*[ConsumerTypeN];
    broker->consumed[FastAlgoDispatch] = new int[RequestTypeN];
    broker->consumed[CostAlgoDispatch] = new int[RequestTypeN];    
    broker->inRequestQueue = new int[RequestTypeN];

    // initialize PC objects
    UniquePC *HDR = new UniquePC();
    UniquePC *RDR = new UniquePC();
    UniquePC *FC = new UniquePC();
    UniquePC *CSC = new UniquePC();

    // add monitor pointers to PC objects
    HDR->broker = broker;
    RDR->broker = broker;
    FC->broker = broker;
    CSC->broker = broker;

    // set flags
    HDR->type = HumanDriver;
    RDR->type = RoboDriver;
    FC->type = FastAlgoDispatch;
    CSC->type = CostAlgoDispatch;

    // default request production limit
    broker->productionLimit = DEFAULT_PROD_LIMIT;

    // check optional arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:c:f:h:a:")) != -1)
    {
        switch (opt)
        {
        // sets total number of requests
        case 'n':
            broker->productionLimit = atoi(optarg);

            // verify production limit is a number
            for (int i = 0; optarg[i] != 0; i++)
            {
                if (!isdigit(optarg[i]))
                {
                    std::cout << "Incorrect production limit--should be a number." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            break;
        // sets time (ms) required by cost saving dispatcher
        case 'c':
            CSC->sleepTime = (float)atoi(optarg) * 1000;

            // verify cost saving dispatch time is a number
            for (int i = 0; optarg[i] != 0; i++)
            {
                if (!isdigit(optarg[i]))
                {
                    std::cout << "Incorrect cost saving dispatch time--should be a number." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            break;
        // sets time (ms) required by fast-matching dispatcher
        case 'f':
            FC->sleepTime = (float)atoi(optarg) * 1000;

            // verify fast-matching dispatch time is a number
            for (int i = 0; optarg[i] != 0; i++)
            {
                if (!isdigit(optarg[i]))
                {
                    std::cout << "Incorrect fast-matching dispatch time--should be a number." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            break;
        // sets time (ms) required to produce a ride request for a human driver
        case 'h':
            HDR->sleepTime = (float)atoi(optarg) * 1000;

            // verify human driver production time is a number
            for (int i = 0; optarg[i] != 0; i++)
            {
                if (!isdigit(optarg[i]))
                {
                    std::cout << "Incorrect human driver production time--should be a number." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            break;
        // sets time (ms) required to produce a ride request for an autonomous car
        case 'a':
            RDR->sleepTime = (float)atoi(optarg) * 1000;

            // verify autonomous driver production time is a number
            for (int i = 0; optarg[i] != 0; i++)
            {
                if (!isdigit(optarg[i]))
                {
                    std::cout << "Incorrect autonomous driver production time--should be a number." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // initialize threads
    pthread_t humanReqProducer;
    pthread_t autoReqProducer;
    pthread_t fastReqConsumer;
    pthread_t costSaveReqConsumer;

    // initialize semaphores
    sem_init(&broker->filledSlots, 0, 0);                 // there are requests in queue
    sem_init(&broker->emptySlots, 0, BUFFER_CAP);         // there is space for requests in queue
    sem_init(&broker->emptyHumanSlots, 0, HUMAN_REQ_CAP); // there is space for human requests in queue
    sem_init(&broker->mutex, 0, 1);                       // mutex for accessing buffer critical section
    sem_init(&broker->precedence, 0, 0);                  // block main thread
    // sem_init(&broker->sleeping, 0, 2);
    
    // create threads for 2 producers and 2 consumers
    pthread_create(&humanReqProducer, NULL, &produce, (void *)HDR);
    pthread_create(&autoReqProducer, NULL, &produce, (void *)RDR);
    pthread_create(&fastReqConsumer, NULL, &consume, (void *)FC);
    pthread_create(&costSaveReqConsumer, NULL, &consume, (void *)CSC);

    // wait for completion of both consumer threads
    sem_wait(&broker->precedence);

    // print summary report
    io_production_report(broker->produced, broker->consumed); 

    // exit main thread
    exit(EXIT_SUCCESS);
}
