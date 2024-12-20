#include "main.h"

error_t getRawData(queue_t *data);
error_t processData(queue_t *data);
error_t printData(queue_t *data);
error_t checkLogs(void);
error_t makeLogFile(const char *log, char *threadName);
int getCoresNumber(void);
