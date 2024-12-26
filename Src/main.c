#include "main.h"

int main()
{

    printf("Getting data...\n");
    

    pthread_t Reader, Analyzer, Printer, Watchdog, Logger;

    int cores = getCoresNumber();


    queue_t cpuData = {};

    queue_t *p_cpuData = &cpuData;
    
    queueInit(&cpuData, cores);
    
    int status;
    status= pthread_create(&Reader, NULL,(void*)readerThread, (void *)p_cpuData);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Analyzer, NULL,(void*)analyzerThread, (void *)p_cpuData);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Printer, NULL,(void*)printnerThread, (void *)p_cpuData);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Logger, NULL,(void*)loggerThread, (void *)p_cpuData);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Watchdog, NULL,(void*)watchdogThread, NULL);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status = pthread_join(Reader, NULL);
    if ( status != 0)
    {
        perror("Thread joining error");
    }

    status =pthread_join(Analyzer,NULL);
    if ( status != 0)
    {
        perror("Thread joining error");
    }
    status =pthread_join(Printer,NULL);
    if ( status != 0)
    {
        perror("Thread joining error");
    }

    status =pthread_join(Watchdog,NULL);
    if ( status != 0)
    {
        perror("Thread joining error");
    }

    status =pthread_join(Logger,NULL);
    if ( status != 0)
    {
        perror("Thread joining error");
    }
    
    queueDeInit(&cpuData);

    pthread_exit(NULL);

    return 0;
}