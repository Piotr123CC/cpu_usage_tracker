#include "../Inc/others.h"
#define FILE_NAME   "/proc/stat"

extern logData_t errors;

error_t getRawData(queue_t *data)
{
    
    memset(errors.reader,0,50);
    FILE *inputFile = NULL;

    inputFile = fopen(FILE_NAME, "r");

    if (inputFile != NULL)
    {
        perror("Openning error");
        strcat(errors.reader,"Opening error, ");
        // return ERROR;
    }
    int i = 0;
    while (i<data->coresNumber)
    {
        if (putIntoQueue(data, inputFile) != OK)
        {
            strcat(errors.reader,"Queue error, ");
            return ERROR;
        }
        i++;
        int c;
        while ( (c = fgetc(inputFile)) != '\n');
    }

    if (fclose(inputFile) != 0)
    {
        perror("Closeing error");
        strcat(errors.reader,"Closeing error");
        return ERROR;
    }
    inputFile = NULL;

    return OK;
}


error_t processData(queue_t *data)
{
    memset(errors.analyzer,0,50);
    if (data->dataSize>=SIZE)
    {
        if (calculateCpuPercentage(data) != OK)
        {
            strcat(errors.analyzer,"Calculating error");
            return ERROR;
        }

        return OK;
    }
}

error_t printData(queue_t *data)
{
    memset(errors.printner,0,50);
    int status;
    status = system("clear");
    if (status != 0)
    {
        strcat(errors.printner,"Clearing console error, ");
        return ERROR;
    }
    for (int i =0;i<data->coresNumber;i++)
    {
        status = printf("%s %.2f \n ",data->data[i].core, data->coresPercentageTable[i]);
        if (status < 1)
        {
            strcat(errors.printner,"Printing error, ");
            return ERROR;
        }
    }
}



int getCoresNumber(void)
{
    int i = 0, cores = 0;

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




error_t checkLogs(void)
{   
    if (errors.reader[0] != '\0')
    {
        makeLogFile(errors.reader);
    } 

    if (errors.analyzer[0] != '\0')
    {
        makeLogFile(errors.reader);
    } 

    if (errors.printner[0] != '\0')
    {
        makeLogFile(errors.reader);
    } 
}


error_t makeLogFile(const char *log)
{
    FILE *outputFile;

    outputFile = fopen("Cpu Usage Tracker log file.txt", "a+");

    if (outputFile == NULL)
    {
        perror("Creating file error");
        return ERROR;
    }

    fputs(log, outputFile);
    
    fprintf(outputFile, "\n");

    if (fclose(outputFile) != 0)
    {
        perror("Closeing file error");
        return ERROR;
    }

    outputFile = NULL;
    return OK;
}


