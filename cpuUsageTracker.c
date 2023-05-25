#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <signal.h>

pthread_mutex_t lock1 ;
volatile sig_atomic_t programStatus =0;




typedef struct{
 	unsigned long   user; 	            //Time spent with normal processing in user mode.
 	unsigned long   nice; 	            //Time spent with niced processes in user mode.
 	unsigned long   system; 	        //Time spent running in kernel mode.
 	unsigned long   idle; 	            //Time spent in vacations twiddling thumbs.
 	unsigned long   iowait; 	        //Time spent waiting for I/O to completed. This is considered idle time too. 	
 	unsigned long   irq; 	            //Time spent serving hardware interrupts. See the description of the intr line for more details. 	
 	unsigned long   softirq; 	        //Time spent serving software interrupts. 	
 	unsigned long   steal; 	            //Time stolen by other operating systems running in a virtual environment. 	
 	unsigned long   guest; 	            //Time spent for running a virtual CPU or guest OS under the control of the kernel. 
    double          cpuUsage;   
    bool            dataAvailable;
    int             cpuCores;

}cpuData1_t;

void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void *CpuDataPassed);
void* loggerThread(void *CpuDataPassed);

int main()
{

    pthread_t Reader, Analyzer, Printer, Watchdog, Logger;
    pthread_create(&Reader, NULL,(void*)readerThread, NULL);
    pthread_create(&Analyzer, NULL,(void*)analyzerThread, NULL);
    pthread_create(&Printer, NULL,(void*)printnerThread, NULL);
    pthread_create(&Watchdog, NULL,(void*)watchdogThread, NULL);
    pthread_create(&Logger, NULL,(void*)loggerThread, NULL);
    pthread_join(Reader,NULL);
    pthread_join(Analyzer,NULL);
    pthread_join(Printer,NULL);
    pthread_join(Watchdog, NULL);
    pthread_join(Logger, NULL);
    pthread_exit(NULL);
    return 0;
}

void* readerThread(void *CpuDataPassed)
{
 
    while (!programStatus)
    {
      
        sleep(1);
    }
  
}


void* analyzerThread(void *CpuDataPassed)
{
   
    while(!programStatus)
    {

        sleep(1);
    }

}


void* printnerThread(void *CpuDataPassed)
{
   
    while (!programStatus)
    {   
       
        sleep(1);
    }
}

void* watchdogThread(void *CpuDataPassed)
{
    
    while(!programStatus)
    {
   
    }
   
}

void* loggerThread(void *CpuDataPassed)
{
  
    while (!programStatus)
    {   

        sleep(1);
    }

}
