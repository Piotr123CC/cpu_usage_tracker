#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "stdio.h"
#include "stdbool.h"

#define SIZE 10

typedef struct cpuData_t{
 	unsigned long   user; 	    //Time spent with normal processing in user mode.
 	unsigned long   nice; 	    //Time spent with niced processes in user mode.
 	unsigned long   system;     //Time spent running in kernel mode.
 	unsigned long   idle; 	    //Time spent in vacations twiddling thumbs.
 	unsigned long   iowait; 	//Time spent waiting for I/O to completed. This is considered idle time too. 	
 	unsigned long   irq; 	    //Time spent serving hardware interrupts. See the description of the intr line for more details. 	
 	unsigned long   softirq; 	//Time spent serving software interrupts. 	
 	unsigned long   steal; 	    //Time stolen by other operating systems running in a virtual environment. 	
 	unsigned long   guest; 	    //Time spent for running a virtual CPU or guest OS under the control of the kernel. 
    double          cpuUsage;   
    bool            dataAvailable;
    char            core[5];

}cpuData_t;


typedef struct{
    cpuData_t  data[SIZE];
    int head, dataSize;
	int coresNumber;
	float *coresPercentageTable;
}queue_t;


void queueInit(queue_t *buffer);

void putIntoQueue(queue_t *buffer, FILE *inputFile);


#endif
