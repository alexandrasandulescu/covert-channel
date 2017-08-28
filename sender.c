#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utils.h"

unsigned long start;
uint32_t frame_id;

// process chunk and send it
void run(char *buf, size_t len, uint32_t frame_id) {
	struct message m;
	create_message(&m, buf, len, frame_id);
	send_message(&m);
}

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s <input-file> <start>\n", argv[0]);
		exit(1);
	}

	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("[S] ERROR: Cannot open file: %s for reading\n", argv[1]);
		exit(1);
	}

	start = atol(argv[2]);
	consume_time(start);
	printf("[S] INFO: Sender starting\n");

	// Read chunks from input file and send across covert channel
	size_t nread;
	frame_id = 0;
	char buf[MAX_MSG_CHARS];
	while ( (nread = fread(buf, 1, MAX_MSG_CHARS, in)) > 0) {
		run(buf, nread, frame_id);
	}

	fclose(in);

	return 0;
}
