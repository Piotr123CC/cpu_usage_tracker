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

    fclose(inputFile);
    inputFile = NULL;
}


void processData(queue_t *data)
{
    if (data->dataSize>=SIZE)
    {
        calculateCpuPercentage(data);
        printf("cpu %.2f \n cpu0 %.2f \n cpu1 %.2f \n cpu2 %.2f \n cpu3 %.2f \n",data->coresPercentageTable[0],
                                                                                 data->coresPercentageTable[1],
                                                                                 data->coresPercentageTable[2],
                                                                                 data->coresPercentageTable[3],
                                                                                 data->coresPercentageTable[4]);
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
    fclose(inputFile);
    inputFile = NULL;
    return cores;
}
