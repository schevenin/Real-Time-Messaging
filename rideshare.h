#ifndef RIDESHARE_H
#define RIDESHARE_H

#include <semaphore.h>
#include <pthread.h>
#include <queue> 

#define HUMAN_REQ_CAP 4
#define BUFFER_CAP 12
#define DEFAULT_PROD_LIMIT 120

/*
 * Arrays with producer and consumer names
 * These can be indexed with the enumerated types below
 * and are defined in io.c
 */
extern const char *producerNames[];
extern const char *producerAbbrevs[];
extern const char *consumerNames[];

/*
 * Enumerated types to be used by the producers and consumers
 * These are expected in the input/output functions (io.h)
 * should be useful in your producer and consumer code.
 */

/**
 * The broker can hold up to a maximum of 12 undispatched requests
 * in its request queue at any given time.
 * To promote the use of autonomous cars (as human drivers cost more),
 * the broker would hold no more than 4 undispatched requests for a human driver at any given time.
 */

/*
 * Two rider request services (producers of requests) are offered:
 *   one for publishing (producing) a ride request for a human driver,
 *   one for publishing (producing) a ride request for an autonomous car.
 * Each ride request service (producer) only publishes (produces)
 * one type of requests,
 * so RequestType is synomonous with the producer type
 */
enum Requests
{
  HumanDriver = 0,  // ride with a human driver
  RoboDriver = 1,   // ride with an autonomous car
  RequestTypeN = 2, // number of ride request types
};

/*
 * Two dispatcher services (consumers of requests) are available using
 * different driver search algorithms
 *   one uses a cost saving algorithm to find a nearby driver
 *   one uses a fast-matching algorithm to find a nearby driver
 *   Requests are taken off from the broker request queue (by dispatchers)
 *   in the order that they are published (produced).
 */
enum Consumers
{
  CostAlgoDispatch = 0, // dispatch to a nearby driver based on cost
  FastAlgoDispatch = 1, // dispatch to a nearby driver based on fast matching
  ConsumerTypeN = 2,    // Number of consumers
};

struct Broker
{
  std::queue <int> buffer; // queue for requests

  // semaphores
  sem_t emptyHumanSlots; // human requests can be produced
  sem_t emptySlots; // requests can be produced
  sem_t filledSlots; // there are requests to be consumed
  sem_t mutex; // access to critical section
  sem_t precedence; // allow termination of main thread

  // counters
  int productionLimit; // request production limit
  int requestsProduced; // total requests produced
  int requestsConsumed; // total requests consumed

  // arrays for output
  int **consumed; // number of requests of each type (HDR and RDR) that have been consumed
  int *produced; // number of requests of each type that have been produced
  int *inRequestQueue; // number of requests of each type that are in the request queue
};

struct UniquePC {
  Broker *broker;
  float sleepTime;
  int type;
};

#endif