#pragma once
#include "main.h"

void getRawData(queue_t *data);
void processData(queue_t *data);
void printData(queue_t *data);
void checkLogs(void);
void makeLogFile(const char *log, char *threadName);
int  getCoresNumber(void);
