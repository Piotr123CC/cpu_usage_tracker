#include "others.h"
#define FILE_NAME   "/proc/stat"

#include "stdlib.h"
#include "stdio.h"

#include <math.h>
extern logData_t errors;

void getRawData(queue_t *data)
{
    
    memset(errors.reader,0,50);
    FILE *inputFile = NULL;

    inputFile = fopen(FILE_NAME, "r");

    if (inputFile == NULL)
    {
        perror("Openning error");
        strcat(errors.reader,"Opening error, ");
    }
    int i = 0;
    while (i<data->coresNumber)
    {
        if (putIntoQueue(data, inputFile) != OK)
        {
            strcat(errors.reader,"Queue error, ");
        }
        i++;
        int c;
        while ( (c = fgetc(inputFile)) != '\n');
    }

    if (fclose(inputFile) != 0)
    {
        perror("Closeing error");
        strcat(errors.reader,"Closeing error");
    }
    inputFile = NULL;

}


void processData(queue_t *data)
{
    memset(errors.analyzer,0,50);
    if (data->actualDataSize>=data->bufferSize)
    {
        if (calculateCpuPercentage(data) != OK)
        {
            strcat(errors.analyzer,"Calculating error");
        }
    }
}

void printData(queue_t *data)
{
    memset(errors.printner,0,50);
    int status;
    status = system("clear");
    if (status != 0)
    {
        strcat(errors.printner,"Clearing console error, ");
    }
    for (int i =0;i<data->coresNumber;i++)
    {
        char used = '#';
        char notUsed = '-';
        if (i == 0)
        {
            status = printf("%s%9.2f %%    ",data->p_data[i].core, data->coresPercentageTable[i]);    
        }
        else if (i >0 && i < 11)
        {
            status = printf("%s%8.2f %%    ",data->p_data[i].core, data->coresPercentageTable[i]);
        }
        else if (i >= 11)
        {
            status = printf("%s%7.2f %%    ",data->p_data[i].core, data->coresPercentageTable[i]);
        }
        printf("[");
        int actualUsed = (int)(data->coresPercentageTable[i]);
        int actualNotUsed = 100 - actualUsed;
        for (int i = 0; i < actualUsed ; i++)
        {
            printf("%c",used);
        }
        for (int i = 0; i < actualNotUsed ; i++)
        {
            printf("%c",notUsed);
        }
        printf("]\n");

        if (status < 1)
        {
            strcat(errors.printner,"Printing error, ");
        }
    }
}



int getCoresNumber(void)
{
    int cores = 0;

    FILE *inputFile = NULL;

    inputFile = fopen(FILE_NAME, "r");

    if (inputFile == NULL)
    {
        perror("File opening error");
        return 0;
    }
    
    char ch[100] ;
    while( fgets(ch,100,inputFile) )
    {
        if (strncmp(ch,"cpu",3) == 0)
        {
            cores++;
        }
    }
    if (fclose(inputFile) != 0)
    {
        perror("Closeing file error");
        return 0;
    }
    inputFile = NULL;
    return cores;
}




void checkLogs(void)
{   
    if (errors.reader[0] != '\0')
    {
        makeLogFile(errors.reader, "Reader: ");
    } 

    if (errors.analyzer[0] != '\0')
    {
        makeLogFile(errors.analyzer, "Analyzer: ");
    } 

    if (errors.printner[0] != '\0')
    {
        makeLogFile(errors.printner, "Printer: ");
    } 
}


void makeLogFile(const char *log, char *threadName)
{
    FILE *outputFile;

    outputFile = fopen("Cpu Usage Tracker log file.txt", "a+");

    if (outputFile == NULL)
    {
        perror("Creating file error");
    }
    fputs(threadName, outputFile);

    fputs(log, outputFile);
    
    fputs("\n",outputFile);

    if (fclose(outputFile) != 0)
    {
        perror("Closeing file error");
    }

    outputFile = NULL;
}


