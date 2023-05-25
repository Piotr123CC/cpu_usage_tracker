cpuUsageTrackerApp: cpuUsageTracker.o others.o
		gcc cpuUsageTracker.o others.o -o cpuUsageTrackerApp

test.o: cpuUsageTracker.c
		gcc -c cpuUsageTracker.c

others.o: others.c others.h
		gcc -c others.c
