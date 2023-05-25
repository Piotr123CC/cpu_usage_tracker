#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <signal.h>
#include "others.h"



void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void *CpuDataPassed);
void* loggerThread(void *CpuDataPassed);



pthread_mutex_t lock1 ;
volatile sig_atomic_t programStatus =0;

logData_t logData = {0,0,0,0,0};


int main()
{

    int cores = getCpuCores();
    assert (cores > 1);
    
    cpuData1_t myCpuData1[cores], *p_myCpuData1;
    p_myCpuData1 = myCpuData1;
    myCpuData1[0].cpuCores = cores;

    signal(SIGINT,interrupt);

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    pthread_t Reader, Analyzer, Printer, Watchdog, Logger;
    pthread_create(&Reader, NULL,(void*)readerThread, (void*)p_myCpuData1);
    pthread_create(&Analyzer, NULL,(void*)analyzerThread, (void*)p_myCpuData1);
    pthread_create(&Printer, NULL,(void*)printnerThread, (void*)p_myCpuData1);
    pthread_create(&Watchdog, NULL,(void*)watchdogThread, (void*)p_myCpuData1);
    pthread_create(&Logger, NULL,(void*)loggerThread, (void*)p_myCpuData1);
    pthread_join(Reader,NULL);
    pthread_join(Analyzer,NULL);
    pthread_join(Printer,NULL);
    pthread_join(Watchdog, NULL);
    pthread_join(Logger, NULL);
    p_myCpuData1 = NULL;
    printf("Memory freed \n");
    pthread_exit(NULL);
    return 0;
}

void* readerThread(void *CpuDataPassed)
{
    cpuData1_t *myData;
    FILE *fp = NULL;
    myData = (cpuData1_t *)CpuDataPassed;
    while (!programStatus)
    {
        pthread_mutex_lock(&lock1);
        if (getRawData(myData, fp) != 0)
        {
            logData.readerError = true;
        }
        else
        {
            logData.readerError = false;
        }
        pthread_mutex_unlock(&lock1);
        sleep(1);
    }
    fp = NULL;
    myData = NULL;
    CpuDataPassed = NULL;
    printf("Reader memory freed\n");
}


void* analyzerThread(void *CpuDataPassed)
{
    cpuData1_t *myData1;

    myData1 = (cpuData1_t *)CpuDataPassed; 

    while(!programStatus)
    {
        pthread_mutex_lock(&lock1);
        
        if (convertData(myData1) != 0)
        {
            logData.analyzerError = true;
        }
        else
        {
            logData.analyzerError = false;            
        }
        pthread_mutex_unlock(&lock1);
        sleep(1);
    }
    myData1 = NULL;
    CpuDataPassed = NULL;
    printf("Analyzer memory freed \n");
}


void* printnerThread(void *CpuDataPassed)
{
    cpuData1_t *myData2;
    myData2 = (cpuData1_t *)CpuDataPassed;
    while (!programStatus)
    {   
        pthread_mutex_lock(&lock1);
        if (myData2[0].dataAvailable == true)
        {
            if (printData(myData2) != 0)
            {
                logData.printerError = true;
                myData2[0].dataAvailable = false;

            }
            else
            {
                logData.printerError = false;
            }
        }
        pthread_mutex_unlock(&lock1);
        sleep(1);
    }
    myData2 = NULL;
    CpuDataPassed = NULL;
    printf("Printer memory freed \n");
}

void* watchdogThread(void *CpuDataPassed)
{
    cpuData1_t *myData3;
    myData3 = (cpuData1_t *)CpuDataPassed;
    signal(SIGALRM, watchdogCallback);
    alarm(2);
    while(!programStatus)
    {
        if (feedWatchdog(myData3) != 0)
        {
            logData.watchdogError = true;
        }
        else
        {
            logData.watchdogError = false;
        }
    }
    myData3 = NULL;
    CpuDataPassed = NULL;
    printf("WatchDog memory freed\n");
}


void* loggerThread(void *CpuDataPassed)
{
    FILE *fp1 = NULL;
    cpuData1_t *mydata4;
    mydata4 = (cpuData1_t *)CpuDataPassed;

    while (!programStatus)
    {   

        pthread_mutex_lock(&lock1);
        if (saveLogData(mydata4, fp1) != 0)
        {
            logData.loggerError = true;
        }
        else
        {
            logData.loggerError = false;
        }
        pthread_mutex_unlock(&lock1);
        sleep(1);
    }
    fp1 = NULL;
    mydata4 = NULL;
    CpuDataPassed = NULL;
    printf("Logger memory free\n");
}


