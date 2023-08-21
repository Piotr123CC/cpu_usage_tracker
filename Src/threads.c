#include "../Inc/threads.h"


static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t printCondition = PTHREAD_COND_INITIALIZER,
                      watchdogCondition = PTHREAD_COND_INITIALIZER;

static bool isDataProcessed = false, isDataPrinted = false;

volatile sig_atomic_t programStatus = 1;

static struct sigaction terminate, interrupt;

logData_t errors = {};

void sigHandler(int signum)
{
    programStatus = 0;
    sleep(1);
    if (signum == SIGTERM)
    {
        printf("Program killed \n");
    }

    if (signum == SIGALRM)
    {
        printf("Timeout... \n");
        strcat(errors.watchDog,"timeout");
        makeLogFile(errors.watchDog,"Watchdog: ");
    }
    if (signum == SIGINT)
    {
        printf("Program was interrupted\n");
    }
    
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&printCondition);
    pthread_cond_destroy(&watchdogCondition);

}


void* readerThread(void *CpuDataPassed)
{
    queue_t *data= (queue_t *)CpuDataPassed;

    terminate.sa_handler = &sigHandler;
    interrupt.sa_handler = &sigHandler;
    sigaction(SIGTERM, &terminate, NULL);
    sigaction(SIGINT, &interrupt, NULL);


    while(programStatus)
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        getRawData(data);
        pthread_mutex_unlock(&lock);
    }
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
    
}


void* printnerThread(void *CpuDataPassed)
{
    queue_t *data = (queue_t *)CpuDataPassed;
    while (programStatus)
    {
        pthread_mutex_lock(&lock);

        while (!isDataProcessed )
        {
            pthread_cond_wait(&printCondition, &lock);
        }
        
        if (data->dataSize>= SIZE)
        {
            printData(data);
        }
        
        isDataProcessed = false;
        isDataPrinted = true;
        pthread_cond_signal(&watchdogCondition);
        pthread_mutex_unlock(&lock);
    }   
    system("clear");
}

void* watchdogThread(void)
{
    alarm(5);
    signal(SIGALRM, sigHandler);
    while (programStatus)
    {
        pthread_mutex_lock(&lock);
        while (!isDataPrinted)
        {
            pthread_cond_wait(&watchdogCondition, &lock);
        }
        alarm(2);
        isDataPrinted = false;
        
        pthread_mutex_unlock(&lock);
    }
}

void* loggerThread(void)
{
    while(programStatus)
    {
        pthread_mutex_lock(&lock);
        checkLogs();
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}