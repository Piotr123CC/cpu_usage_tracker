#include "../Inc/main.h"

void queueInit(queue_t *buffer, const int coresNumber)
{
    buffer->coresNumber = coresNumber;
    buffer->dataSize=0;
    buffer->head = 0;

    for (size_t i = 0;i<SIZE;i++)
    {
        buffer->data[i].cpuUsage = 0;
        buffer->data[i].guest = 0;
        buffer->data[i].idle = 0;
        buffer->data[i].iowait = 0;
        buffer->data[i].irq = 0;
        buffer->data[i].nice = 0;
        buffer->data[i].softirq = 0;
        buffer->data[i].steal = 0;
        buffer->data[i].system = 0;
        buffer->data[i].user = 0;
    }

    buffer->p_data = buffer->data;
    buffer->coresPercentageTable = (float *)calloc(buffer->coresNumber, sizeof(float));
}

error_t putIntoQueue(queue_t *buffer, FILE *inputFile)
{

    int i = buffer->head;
    if (buffer->dataSize <= SIZE)
    {   
        buffer->dataSize++;
    }

    int status;
    status = fscanf(inputFile,"%s %ld %ld %ld %ld %ld %ld %ld %ld %ld ",(buffer->data[i].core),&(buffer->data[i].user),
                                                              &(buffer->data[i].nice), &(buffer->data[i].system),
                                                              &(buffer->data[i].idle), &(buffer->data[i].iowait),
                                                              &(buffer->data[i].irq), &(buffer->data[i].softirq),
                                                              &(buffer->data[i].steal), &(buffer->data[i].guest));

    if (status < 10)
    {
        return ERROR;
    }

    buffer->head = ++(buffer->head) % SIZE; 

    return OK;
}

void queueDeInit(queue_t *buffer)
{
    buffer->dataSize=0;
    buffer->head = 0;
    free(buffer->coresPercentageTable);
    buffer->p_data = NULL;    
}
