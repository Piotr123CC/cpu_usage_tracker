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

int getCpuCores();
void watchdogCallabck(int sig);


int main()
{

    int cores = getCpuCores();
    assert (cores > 1);
    
    cpuData1_t myCpuData1[cores], *p_myCpuData1;

    p_myCpuData1 = myCpuData1;
    myCpuData1[0].cpuCores = cores;
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
        fp = fopen(FILENAME,"r");
        if (fp == NULL)
        {
            return 0;
        }
        int charCounter =0;
        char ch;
        char name[20];
        if (fp == NULL)
        {

            myData[0].dataAvailable = false;
        }
        else 
        {
            charCounter =0;
        
            while ( charCounter < myData[0].cpuCores)
            {
                fscanf(fp,"%s %ld %ld %ld %ld %ld %ld %ld %ld %ld",name, &(myData[charCounter].user), &(myData[charCounter].nice), &(myData[charCounter].system),
                                                                                &(myData[charCounter].idle), &(myData[charCounter].iowait), &(myData[charCounter].irq),
                                                                                &(myData[charCounter].softirq), &(myData[charCounter].steal), &(myData[charCounter].guest));

                while ((ch = fgetc(fp)) != '\n' );

                charCounter++;
            }
        

        fclose(fp);
        fp = NULL;
        }
        pthread_mutex_unlock(&lock1);
        sleep(1);
    }
   
}


void* analyzerThread(void *CpuDataPassed)
{
    cpuData1_t *myData1;

    myData1 = (cpuData1_t *)CpuDataPassed; 
   
    while(!programStatus)
    {

        static unsigned long prevIdle[5], prevSum[5];
        unsigned long currentSum[5];

        pthread_mutex_lock(&lock1);
        for (int i =0; i < myData1[0].cpuCores;i++)
        {
            currentSum[i]= (myData1[i].user+myData1[i].nice+myData1[i].system+myData1[i].idle+
                            myData1[i].iowait+myData1[i].irq+myData1[i].softirq);
            
            if (prevSum[i] == currentSum[i])
            {
                myData1[0].dataAvailable = false;
                break;
            }
            myData1[i].cpuUsage = 100.0 - (myData1[i].idle - prevIdle[i])*100.0/(currentSum[i]-prevSum[i]);
            prevIdle[i] = myData1[i].idle;
            prevSum[i] = currentSum[i];
            myData1[0].dataAvailable = true;
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
        static double prevTotal = 0;
        system("clear");
        for (int i = 0;i< myData2[0].cpuCores;i++)
        {
            
            if (i == 0)
            {
                printf("Cpu total   %lf  \n",myData2[i].cpuUsage);
                if (myData2[i].cpuUsage == 0.0000000000000)
                {
                    break;
                }
                else if (prevTotal ==  myData2[0].cpuUsage) 
                {
                    break;
                }
            }
            else
            {
                printf("Cpu %d       %lf  \n",i,myData2[i].cpuUsage);
            }
    }
    prevTotal = myData2[0].cpuUsage; 
    sleep(1);
    }
}

void* watchdogThread(void *CpuDataPassed)
{
    cpuData1_t *myData3;
    myData3 = (cpuData1_t *)CpuDataPassed;
    signal(SIGALRM, watchdogCallabck);
    alarm(2);
    while(!programStatus)
    {
        if (myData3[0].dataAvailable == true)
        {
            alarm(2);
        }
        usleep(250000);
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
        fp1 = fopen("logData.txt","a");
        if (fp1 == NULL)
        {
        printf("Logger         ERROR\n");

        }
        else
        {
            if ((mydata4[0].dataAvailable == true))
            {
                
                for (int i = 0;i< mydata4[0].cpuCores;i++)
                {
                    if (i == 0)
                    {
                        fprintf(fp1,"%s          %lf\n","cpu",mydata4[i].cpuUsage);
                    }
                    else
                    {
                        fprintf(fp1,"%s%d         %lf\n","cpu",i,mydata4[i].cpuUsage);
                    }
                }
            }
            fprintf(fp1,"%s \n","----------------------------------------------------------");
            fclose(fp1);
            fp1 = NULL;
        }
        pthread_mutex_unlock(&lock1);
        sleep(1);
    }

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


void watchdogCallabck(int sig)
{
    printf("\nProgram will be closed, timeout... \n");
    exit(1);
}
