#include "main.h"

void queueInit(queue_t *buffer, const int coresNumber)
{
    buffer->coresNumber = coresNumber;
    buffer->bufferSize = 2 * coresNumber;
    buffer->actualDataSize = 0;
    buffer->head = 0;

    buffer->p_data = (cpuData_t *)calloc(buffer->bufferSize, sizeof(cpuData_t));
    
    for (size_t i = 0;i < buffer->bufferSize;i++)
    {
        buffer->p_data[i].cpuUsage = 0;
        buffer->p_data[i].guest = 0;
        buffer->p_data[i].idle = 0;
        buffer->p_data[i].iowait = 0;
        buffer->p_data[i].irq = 0;
        buffer->p_data[i].nice = 0;
        buffer->p_data[i].softirq = 0;
        buffer->p_data[i].steal = 0;
        buffer->p_data[i].system = 0;
        buffer->p_data[i].user = 0;
    }

    buffer->p_data = buffer->p_data;
    buffer->coresPercentageTable = (float *)calloc(buffer->coresNumber, sizeof(float));
}

error_t putIntoQueue(queue_t *buffer, FILE *inputFile)
{

    int i = buffer->head;
    if (buffer->actualDataSize <= buffer->bufferSize)
    {   
        buffer->actualDataSize++;
    }

    int status;
    status = fscanf(inputFile,"%s %ld %ld %ld %ld %ld %ld %ld %ld %ld ",(buffer->p_data[i].core),&(buffer->p_data[i].user),
                                                              &(buffer->p_data[i].nice), &(buffer->p_data[i].system),
                                                              &(buffer->p_data[i].idle), &(buffer->p_data[i].iowait),
                                                              &(buffer->p_data[i].irq), &(buffer->p_data[i].softirq),
                                                              &(buffer->p_data[i].steal), &(buffer->p_data[i].guest));

    if (status < 10)
    {
        return ERROR;
    }

    buffer->head = ++(buffer->head) % buffer->bufferSize; 

    return OK;
}

void queueDeInit(queue_t *buffer)
{
    buffer->actualDataSize=0;
    buffer->head = 0;
    free(buffer->coresPercentageTable);
    free(buffer->p_data);
        
}
