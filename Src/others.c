#include "../Inc/others.h"

#define FILE_NAME   "/proc/stat"

void getRawData(queue_t *data)
{
    FILE *inputFile = NULL;

    inputFile = fopen(FILE_NAME, "r");

    if (inputFile == 0)
    {
        perror("Openning error");
    }
    int i = 0;
    while (i<data->coresNumber)
    {
        putIntoQueue(data, inputFile);
        i++;
        int c;
        while ( (c = fgetc(inputFile)) != '\n');
    }

    if (fclose(inputFile) != 0)
    {
        perror("Closeing error");
    }
    inputFile = NULL;
}


void processData(queue_t *data)
{
    if (data->dataSize>=SIZE)
    {
        calculateCpuPercentage(data);
    }
}

void printData(queue_t *data)
{
    system("clear");
    for (int i =0;i<data->coresNumber;i++)
    {
        printf("%s %.2f \n ",data->data[i].core, data->coresPercentageTable[i]);
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
    }
    inputFile = NULL;
    return cores;
}

