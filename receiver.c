#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

unsigned long start;
unsigned int frame_id = 0;

void run(char *buf, size_t *len) {
	// receive buffer across covert channel
	struct message m;
	recv_message(&m);
	if (m.frame_id > frame_id) {
		frame_id = m.frame_id;
	}

	if (m.len > MAX_MSG_CHARS) {
		printf("ERROR: Msg transmission error. Len: %u frame: %u crc: %u\n",
				m.len, m.frame_id, m.crc32);
		exit(1);
	}
	strncpy(buf, m.payload, m.len);
	*len = m.len;
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

	start = atol(argv[2]) * 1000000L;
	unsigned long quantum = get_rr_time_quantum();
	printf("[R] INFO: Receiver starting. Quantum: %lu\n", quantum);
	consume_time(start);

	while (true) {
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
