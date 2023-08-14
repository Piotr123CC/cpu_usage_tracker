#include "../Inc/threads.h"


pthread_mutex_t lock;

void* readerThread(void *CpuDataPassed)
{
    queue_t *data= (queue_t *)CpuDataPassed;
    while(1)
    {

        sleep(1);
        pthread_mutex_lock(&lock);
        printf("Reader -----\n");
        getRawData(data);
        pthread_mutex_unlock(&lock);
    }
}



void* analyzerThread(void *CpuDataPassed)
{
    queue_t *data = (queue_t *)CpuDataPassed;
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        printf("Analyzer -----\n");
        processData(data);
        
        pthread_mutex_unlock(&lock);

    }
}
