/**
 * @file driver.cpp
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

    // initialize PC objects
    UniquePC *HR = new UniquePC();
    UniquePC *AR = new UniquePC();
    UniquePC *FC = new UniquePC();
    UniquePC *CSC = new UniquePC();

    // add monitor pointers to PC objects
    HR->broker = broker;
    AR->broker = broker;
    FC->broker = broker;
    CSC->broker = broker;

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
            CSC->sleepTime = atoi(optarg);

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
            FC->sleepTime = atoi(optarg);

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
            HR->sleepTime = atoi(optarg);

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
            AR->sleepTime = atoi(optarg);

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

    //initialize semaphores
    sem_init(&broker->emptyHumanSlots, 0, 1);
    sem_init(&broker->filledSlots, 0, 0);
    sem_init(&broker->emptySlots, 0, broker->bufferCapacity);

    // create threads for 2 producers and 2 consumers
    pthread_create(&humanReqProducer, NULL, &produce, (void *) HR);
    pthread_create(&autoReqProducer, NULL, &produce, (void *) AR);
    pthread_create(&fastReqConsumer, NULL, &consume, (void *) FC);
    pthread_create(&costSaveReqConsumer, NULL, &consume, (void *) CSC);    
}

