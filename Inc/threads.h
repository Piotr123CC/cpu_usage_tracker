#ifndef _THREADS_H_
#define _THREADS_H_

#include "main.h"
#include <stdlib.h>
#include <signal.h>

typedef struct {
    char reader[30];
    char analyzer[30];
    char printner[30];
}logData_t;

void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void);
void* loggerThread(void);


#endif