#include "../Inc/algorithm.h"



error_t calculateCpuPercentage(queue_t *data)
{
    float percentage = 0;
    int current, previous ;
 
    for (int i=0;i< SIZE/2;i++)
    {
        current =  (data->head + i + SIZE/2) % SIZE;
        previous = (data->head + i) % SIZE;


        unsigned long long prevIdle = data->data[previous].idle + data->data[previous].iowait;

        unsigned long long Idle = data->data[current].idle + data->data[current].iowait;


        unsigned long long prevNonIdle = data->data[previous].user + data->data[previous].nice
                                            + data->data[previous].system + data->data[previous].irq
                                            + data->data[previous].softirq + data->data[previous].steal;
                                    
        unsigned long long NonIdle = data->data[current].user + data->data[current].nice
                                        + data->data[current].system + data->data[current].irq
                                        + data->data[current].softirq + data->data[current].steal;


        unsigned long long prevTotal = prevIdle + prevNonIdle;

        unsigned long long Total = Idle + NonIdle;


        unsigned long long totald = Total - prevTotal;

        unsigned long long idled = Idle - prevIdle;


        percentage = 100.0 - (idled*100.00/(float)totald);
        if (percentage < 0.01 || percentage > 100.0)
        {
            return ERROR;
        }
        data->coresPercentageTable[i] = percentage;
        float cpu = data->coresPercentageTable[i];
        
    }
    return OK;
}