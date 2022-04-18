#ifndef PRODUCER_H
#define PRODUCER_H

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "rideshare.h"
#include "io.h"

void *produce(void *);

#endif