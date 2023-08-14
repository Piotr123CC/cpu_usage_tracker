#include "../Inc/queue.h"





void queueInit(queue_t *buffer)
{
    buffer->dataSize=0;
    buffer->head = 0;
    for (size_t i = 0;i<SIZE;i++)
    {
        // buffer->data[i].core = 0;
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
}

void putIntoQueue(queue_t *buffer, FILE *inputFile)
{

    int i = buffer->head;
    if (buffer->dataSize <= SIZE)
    {   
        buffer->dataSize++;
    }


    fscanf(inputFile,"%s %ld %ld %ld %ld %ld %ld %ld %ld %ld ",(buffer->data[i].core),&(buffer->data[i].user),
                                                              &(buffer->data[i].nice), &(buffer->data[i].system),
                                                              &(buffer->data[i].idle), &(buffer->data[i].iowait),
                                                              &(buffer->data[i].irq), &(buffer->data[i].softirq),
                                                              &(buffer->data[i].steal), &(buffer->data[i].guest));

    buffer->head = ++(buffer->head) % SIZE; 

}