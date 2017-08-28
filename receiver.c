#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

unsigned long start;

void run(char *buf, size_t *len) {
	// receive buffer across covert channel
	strncpy(buf, "ANA", 4);
	*len = 4;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s <output-file> <start>\n", argv[0]);
		exit(1);
	}

	FILE *out = fopen(argv[1], "w+");
	if (out == NULL) {
		printf("[S] ERROR: Cannot open file: %s for writing\n", argv[1]);
		exit(1);
	}

	start = atol(argv[2]);
	consume_time(start);
	printf("[R] INFO: Receiver starting\n");

	int times = 0;
	while (true) {
		times += 1;
		if (times >= 10)
			break;

		char buf[MAX_MSG_CHARS];
		size_t len;

		run(buf, &len);

		size_t res = 0;
		size_t nwritten = 0;

		while ((nwritten < len) &&
				(res = fwrite(buf + nwritten, 1, len - nwritten, out)) > 0) {
			nwritten += res;
		}
	}

	printf("[R] INFO: Receiver exiting with success\n");

	return 0;
}
