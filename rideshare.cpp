/**
 * @file rideshare.cpp
 * @authors Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Real-time Messaging for Ridesharing entry point
 * @date 2022-04-21
 */

#include "rideshare.h"

/**
 * @brief Main Execution of Real-time Messaging for Ridesharing
 *
 * @param argc integer amount of arguments
 * @param argv character array of arguments
 * @return int value indicating exit status
 */
int main(int argc, char **argv)
{

    int productionLimit = 120;
    int costSavingDispatchTime = 0;
    int fastMatchingDispatchTime = 0;
    int humanDriverProductionTime = 0;
    int autoDriverProductionTime = 0;


    // check optional arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:c:f:h:a:")) != -1)
    {
        switch (opt)
        {
        // sets total number of requests
        case 'n':
            productionLimit = atoi(optarg);

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
            costSavingDispatchTime = atoi(optarg);

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
            fastMatchingDispatchTime = atoi(optarg);

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
            humanDriverProductionTime = atoi(optarg);

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
            autoDriverProductionTime = atoi(optarg);

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
}