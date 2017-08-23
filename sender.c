#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// process chunk and send it
void run(char *buf, size_t len) {
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <input-file>\n", argv[0]);
		exit(1);
	}

	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("[S] ERROR: Cannot open file: %s for reading\n", argv[1]);
		exit(1);
	}

	printf("[S] INFO: Sender starting\n");

	// Read chunks from input file and send across covert channel
	size_t nread;
	char buf[MAX_MSG_CHARS];
	while ( (nread = fread(buf, 1, MAX_MSG_CHARS, in)) > 0) {
		run(buf, nread);
	}

	fclose(in);

	return 0;
}
