#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


typedef enum threadError_t{
    threadOK, 
    readerError,
    analyzerError,
    printerError,
    watchdogError,
    loggerError,
}threadError_t;

typedef struct{
    bool readerError;
    bool analyzerError;   
    bool printerError;   
    bool watchdogError;   
    bool loggerError;     
}logData_t;


typedef struct{
 	unsigned long   user; 	    //Time spent with normal processing in user mode.
 	unsigned long   nice; 	    //Time spent with niced processes in user mode.
 	unsigned long   system; 	    //Time spent running in kernel mode.
 	unsigned long   idle; 	    //Time spent in vacations twiddling thumbs.
 	unsigned long   iowait; 	    //Time spent waiting for I/O to completed. This is considered idle time too. 	
 	unsigned long   irq; 	    //Time spent serving hardware interrupts. See the description of the intr line for more details. 	
 	unsigned long   softirq; 	//Time spent serving software interrupts. 	
 	unsigned long   steal; 	    //Time stolen by other operating systems running in a virtual environment. 	
 	unsigned long   guest; 	    //Time spent for running a virtual CPU or guest OS under the control of the kernel. 
    double          cpuUsage;   
    bool            dataAvailable;
    int             cpuCores;

}cpuData1_t;


int getCpuCores();
void watchdogCallabck(int sig);
void interrupt(int sig);
void term(int sig);
threadError_t printData(cpuData1_t *convertedData1);
threadError_t getRawData(cpuData1_t *fileData, FILE *handle);
threadError_t convertData(cpuData1_t *rawData);
threadError_t saveLogData(cpuData1_t *convertedData, FILE *fp);
threadError_t feedWatchdog(cpuData1_t *isDataAvailable);
