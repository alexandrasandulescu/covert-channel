#include <stdio.h>
#include <sys/time.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"

unsigned long time_in_s(unsigned long time_in_us) {
	return time_in_us / 1000000L;
}

int main(int argc, char **argv) {
	unsigned long start_time = 0;

	if (argc == 2) {
		start_time = atol(argv[1]) * 1000000L;
		printf("Starting measurements at: %s. Current time: %lu\n",
				argv[1], time_in_s(current_time()));
		consume_time(start_time);
	}

	volatile unsigned long long i;
	unsigned long start, stop, diff;

	start = current_time();
	for (i = 0; i < OPS; ++i);
	stop = current_time();

	printf("Consumed: %lu (us)\n", stop - start);
	unsigned long quantum = get_rr_time_quantum();

	if (quantum != TASK_TIME) {
		printf("You are using different quantum %lu\n", TASK_TIME);
	}
	printf("Time quantum: %lu (us)\n", quantum);
	return 0;
}
