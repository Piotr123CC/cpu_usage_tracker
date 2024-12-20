#pragma once

#include "main.h"
#include "stdlib.h"
#include <features.h>
#include <signal.h>
#include "pthread.h"


typedef struct {
    char reader[100];
    char analyzer[100];
    char printner[50];
    char watchDog[50];
}logData_t;


void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void);
void* loggerThread(void);
