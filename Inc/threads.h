#ifndef _THREADS_H_
#define _THREADS_H_

#include "main.h"
#include <stdlib.h>
#include <signal.h>
void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void *CpuDataPassed);
void* loggerThread(void *CpuDataPassed);


#endif