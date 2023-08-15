#include "../Inc/main.h"

int main()
{
    pthread_t Reader, Analyzer, Printer, Watchdog, Logger;

    queue_t cpuData = {}, *p_cpuData;
    cpuData.coresNumber = getCoresNumber();
    cpuData.coresPercentageTable = (float *)calloc(cpuData.coresNumber, sizeof(float));
    queueInit(&cpuData);
    p_cpuData = &cpuData;
    pthread_create(&Reader, NULL,(void*)readerThread, (void *)p_cpuData);
    pthread_create(&Analyzer, NULL,(void*)analyzerThread, (void *)p_cpuData);
    pthread_create(&Printer, NULL,(void*)printnerThread, (void *)p_cpuData);
    pthread_join(Reader, NULL);
    pthread_join(Analyzer,NULL);
    pthread_join(Printer,NULL);
    free(cpuData.coresPercentageTable);
    pthread_exit(NULL);
    
    cpuData.coresPercentageTable = 0;
    p_cpuData = NULL;
    sleep(1);
    printf("Memory freed \n");
    return 0;
}