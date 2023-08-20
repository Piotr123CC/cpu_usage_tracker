#ifndef _OTHERS_H_
#define _OTHERS_H_

#include "main.h"



error_t getRawData(queue_t *data);
error_t processData(queue_t *data);
error_t printData(queue_t *data);
int getCoresNumber(void);
error_t checkLogs(void);
error_t makeLogFile(const char *log);


#endif