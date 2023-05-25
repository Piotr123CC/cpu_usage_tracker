#include "others.h"


#define FILENAME "/proc/stat"

extern logData_t logData;
extern sig_atomic_t programStatus;

int getCpuCores()
{
    FILE *fp = NULL;
    fp = fopen(FILENAME,"r");
    if (fp == NULL)
    {
        return 0;
    }
    int coreNumber =0;
    char str[60];
    char cpu[] = "cpu";
    while (fgets(str,60,fp))
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


void term(int sig)
{
    programStatus = 1;
  
}

void interrupt(int sig)
{
    printf("Program was interrupted\n");
    raise(SIGTERM);
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




