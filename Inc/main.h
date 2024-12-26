#pragma once
#define _ISOC99_SOURCE

typedef enum 
{
    OK,
    ERROR
}error_t;

#include "stdio.h"
#include "pthread.h"
#include "malloc.h"
#include "unistd.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"
#include <signal.h>

#include "queue.h"
#include "others.h"
#include "algorithm.h"
#include "mythreads.h"
