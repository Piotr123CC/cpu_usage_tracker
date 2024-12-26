#include "algorithm.h"



error_t calculateCpuPercentage(queue_t *data)
{
    double percentage = 0;
    int current = 0, previous = 0 ;
 
    for (int i=0;i< data->coresNumber;i++)
    {
        current =  (data->head + i + data->coresNumber) % data->bufferSize;
        previous = (data->head + i) % data->bufferSize;


        unsigned long long prevIdle = data->p_data[previous].idle + data->p_data[previous].iowait;

        unsigned long long Idle = data->p_data[current].idle + data->p_data[current].iowait;


        unsigned long long prevNonIdle = data->p_data[previous].user + data->p_data[previous].nice
                                            + data->p_data[previous].system + data->p_data[previous].irq
                                            + data->p_data[previous].softirq + data->p_data[previous].steal;
                                    
        unsigned long long NonIdle = data->p_data[current].user + data->p_data[current].nice
                                        + data->p_data[current].system + data->p_data[current].irq
                                        + data->p_data[current].softirq + data->p_data[current].steal;


        unsigned long long prevTotal = prevIdle + prevNonIdle;

        unsigned long long Total = Idle + NonIdle;


        unsigned long long totald = Total - prevTotal;

        unsigned long long idled = Idle - prevIdle;
        percentage = 100.0 - ((double)idled*100.00/(double)totald);
        if (percentage < 0.00 || percentage > 100.0)
        {
            return ERROR;
        }
        data->coresPercentageTable[i] = percentage;        
    }
    return OK;
}