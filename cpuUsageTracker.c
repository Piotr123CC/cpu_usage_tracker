#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <signal.h>

#define FILENAME "/proc/stat"

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

typedef enum threadError_t{
    threadOK, 
    readerError,
    analyzerError,
    printerError,
    watchdogError,
    loggerError,
}threadError_t;

typedef struct{
    bool readerError;
    bool analyzerError;   
    bool printerError;   
    bool watchdogError;   
    bool loggerError;     
}logData_t;



void* readerThread(void *CpuDataPassed);
void* analyzerThread(void *CpuDataPassed);
void* printnerThread(void *CpuDataPassed);
void* watchdogThread(void *CpuDataPassed);
void* loggerThread(void *CpuDataPassed);

int getCpuCores();
void watchdogCallback(int sig);
threadError_t printData(cpuData1_t *convertedData1);
threadError_t getRawData(cpuData1_t *fileData, FILE *handle);
threadError_t convertData(cpuData1_t *rawData);
threadError_t saveLogData(cpuData1_t *convertedData, FILE *fp);
threadError_t feedWatchdog(cpuData1_t *isDataAvailable);
// void interrupt(int sig);
// void term(int sig);


pthread_mutex_t lock1 ;
volatile sig_atomic_t programStatus =0;

logData_t logData = {0,0,0,0,0};

void term(int sig)
{
    printf("Program was termianted\n");
    sleep(2);
    programStatus = 1;

}

void interrupt(int sig)
{
    printf("Program was interrupted\n");
    raise(SIGTERM);
}


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
    fclose(fp);
    fp = NULL;
    
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
  
}


int getCpuCores()
{
    FILE *fp = NULL;
    fp = fopen(FILENAME,"r");
    if (fp == NULL)
    {
        return 0;
    }
    int coreNumber =0;
    char str[80];
    char cpu[] = "cpu";
    while (fgets(str,80,fp))
    {   
        if (strncmp(cpu,str,3) == 0)
        {
            coreNumber++;
        }
    }
    fclose(fp);
    fp = NULL;
    return coreNumber;
}


threadError_t feedWatchdog(cpuData1_t *isDataAvailable)
{
    if (isDataAvailable[0].dataAvailable == true)
        {
            int seconds = alarm(2);
           if ( seconds < 0 )
           {
                return logData.watchdogError;
           }
        }
        usleep(250000);
}

void watchdogCallback(int sig)
{
    printf("Error code: ");
    if (logData.readerError)
    {
        printf("%d",logData.readerError);
    }
    else
    {
        printf("0");
    }

    if (logData.analyzerError)
    {
        printf("%d",logData.analyzerError);
    }
    else
    {
        printf("0");
    }

    if (logData.printerError)
    {
        printf("%d",logData.printerError);
    }
    else
    {
        printf("0");
    }

    if (logData.loggerError)
    {
        printf("%d",logData.loggerError);
    }
    else
    {
        printf("0");
    }

    if (logData.readerError)
    {
        printf("%d",logData.readerError);
    }
    else
    {
        printf("0");
    }
    printf("\nProgram will be closed, timeout... \n");
    exit(1);
}


threadError_t printData(cpuData1_t *convertedData)
{
    static double prevTotal = 0;
    system("clear");
    for (int i = 0;i< convertedData[0].cpuCores;i++)
    {
        
        if (i == 0)
        {
            printf("Cpu total   %lf  \n",convertedData[i].cpuUsage);
            if (convertedData[i].cpuUsage == 0.0000000000000)
            {
                return printerError;
            }
            else if (prevTotal ==  convertedData[0].cpuUsage) 
            {
                return printerError;
            }
        }
        else
        {
            printf("Cpu %d       %lf  \n",i,convertedData[i].cpuUsage);
        }
    }
    prevTotal = convertedData[0].cpuUsage; 
    return threadOK;   
}

threadError_t getRawData(cpuData1_t *fileData, FILE *handle)
{
    handle = fopen(FILENAME, "r");
     
    int charCounter =0;
    char ch;
    char name[20];
    if (handle == NULL)
    {
        
        fileData[0].dataAvailable = false;
        return readerError;
    }
    else 
    {
        charCounter =0;
       
        while ( charCounter < fileData[0].cpuCores)
        {
            fscanf(handle,"%s %ld %ld %ld %ld %ld %ld %ld %ld %ld",name, &(fileData[charCounter].user), &(fileData[charCounter].nice), &(fileData[charCounter].system),
                                                                            &(fileData[charCounter].idle), &(fileData[charCounter].iowait), &(fileData[charCounter].irq),
                                                                            &(fileData[charCounter].softirq), &(fileData[charCounter].steal), &(fileData[charCounter].guest));

            while ((ch = fgetc(handle)) != '\n' );

            charCounter++;
        }

    fclose(handle);
    handle = NULL;
    }

    return threadOK;
}

threadError_t convertData(cpuData1_t *rawData)
{
    static unsigned long prevIdle[5], prevSum[5];
    unsigned long currentSum[5];
    for (int i =0; i < rawData[0].cpuCores;i++)
    {
        currentSum[i]= (rawData[i].user+rawData[i].nice+rawData[i].system+rawData[i].idle+
                        rawData[i].iowait+rawData[i].irq+rawData[i].softirq);
        
        if (prevSum[i] == currentSum[i])
        {
            rawData[0].dataAvailable = false; 
            return analyzerError;
        }
        rawData[i].cpuUsage = 100.0 - (rawData[i].idle - prevIdle[i])*100.0/(currentSum[i]-prevSum[i]);

        if (rawData[i].cpuUsage <= 0)
        {
            return analyzerError;
        }
        prevIdle[i] = rawData[i].idle;
        prevSum[i] = currentSum[i];
        rawData[0].dataAvailable = true;
    }
    return threadOK;
}


threadError_t saveLogData(cpuData1_t *convertedData, FILE *fp)
{
    fp=fopen("logData.txt","a");

    if (fp == NULL)
    {
        printf("Logger         ERROR\n");
        return loggerError;
    }
    else
    {
        if ((convertedData[0].dataAvailable == true))
        {
            
            for (int i = 0;i< convertedData[0].cpuCores;i++)
            {
                if (i == 0)
                {
                    fprintf(fp,"%s          %lf\n","cpu",convertedData[i].cpuUsage);
                }
                else
                {
                    fprintf(fp,"%s%d         %lf\n","cpu",i,convertedData[i].cpuUsage);
                }
            }
        }
        fprintf(fp,"%s \n","----------------------------------------------------------");
        if (logData.readerError == true)
        {
            fprintf(fp,"%s           %s\n","Reader","ERROR");
        }
        else
        {
            fprintf(fp,"%s           %s\n","Reader","OK");
        }

        if (logData.analyzerError == true)
        {
            fprintf(fp,"%s         %s\n","Analyzer","ERROR");
        }
        else
        {
            fprintf(fp,"%s         %s\n","Analyzer","OK");
        }

        if (logData.printerError == true)
        {
            fprintf(fp,"%s          %s\n","Printer","ERROR");
        }
        else
        {
            fprintf(fp,"%s          %s\n","Printer","OK");
        }

        if (logData.watchdogError == true)
        {
            fprintf(fp,"%s         %s\n","Watchdog","ERROR");
        }
        else
        {
            fprintf(fp,"%s         %s\n","Watchdog","OK");
        }

        fprintf(fp,"%s           %s\n","Logger","OK");

        fprintf(fp,"%s \n","------------------------------------------------------------------------------------");
        fclose(fp);
        fp = NULL;
        return threadOK;
    }
    
}


