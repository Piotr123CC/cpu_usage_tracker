#ifndef _THREADS_H_
#define _THREADS_H_

#include "main.h"
#include <stdlib.h>
#include <signal.h>

typedef struct {
    char reader[100];
    char analyzer[100];
    char printner[100];
    char watchDog[50];
}logData_t;


void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void);
void* loggerThread(void);


#endif