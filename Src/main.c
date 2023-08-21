#include "../Inc/main.h"



int main()
{

    printf("Getting data...\n");
    

    pthread_t Reader, Analyzer, Printer, Watchdog, Logger;

    queue_t cpuData = {} ;

    
    queueInit(&cpuData, getCoresNumber());
    
    int status;
    status= pthread_create(&Reader, NULL,(void*)readerThread, (void *)cpuData.p_data);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Analyzer, NULL,(void*)analyzerThread, (void *)cpuData.p_data);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Printer, NULL,(void*)printnerThread, (void *)cpuData.p_data);
    if ( status != 0)
    {
        perror("Thread creating error");
    }

    status=pthread_create(&Logger, NULL,(void*)loggerThread, (void *)cpuData.p_data);
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