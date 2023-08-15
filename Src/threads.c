#include "../Inc/threads.h"


static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t printCondition = PTHREAD_COND_INITIALIZER;

static bool isDataProcessed = false;

volatile sig_atomic_t programStatus = 1;

static struct sigaction action;

void sigHandler(int signum)
{
    if (signum == SIGTERM)
    {
        
        programStatus = 0;
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&printCondition);
        sleep(1);
        printf("Mutex and cond destroy\n");
    }
}


void* readerThread(void *CpuDataPassed)
{
    queue_t *data= (queue_t *)CpuDataPassed;
    // action.sa= 
    action.sa_handler = &sigHandler;
    sigaction(SIGTERM, &action, NULL);
    while(programStatus)
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        getRawData(data);
        pthread_mutex_unlock(&lock);
    }
    printf("close reader thread \n");
}



void* analyzerThread(void *CpuDataPassed)
{
    queue_t *data = (queue_t *)CpuDataPassed;
    while(programStatus)
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        processData(data);
        isDataProcessed = true;
        pthread_cond_signal(&printCondition);
        pthread_mutex_unlock(&lock);

    }

    printf("Close analyzer thread \n");
}


void* printnerThread(void *CpuDataPassed)
{
    queue_t *data = (queue_t *)CpuDataPassed;
    while (programStatus)
    {
        pthread_mutex_lock(&lock);

        while (!isDataProcessed)
        {
            pthread_cond_wait(&printCondition, &lock);
        }
        printData(data);
        isDataProcessed = false;
        pthread_mutex_unlock(&lock);
    }   
    sleep(1);
    printf("Close print thread \n");
}
