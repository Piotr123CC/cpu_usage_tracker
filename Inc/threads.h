#ifndef _THREADS_H_
#define _THREADS_H_

#include "main.h"
#include <stdlib.h>
#include <signal.h>

typedef struct {
    char reader[50];
    char analyzer[50];
    char printner[50];
}logData_t;


void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void);
void* loggerThread(void);


#endif