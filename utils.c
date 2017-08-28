#include "utils.h"

#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

extern unsigned int xcrc32(const unsigned char *, int, unsigned int);
unsigned long current_time();

bool check_ack(struct message *m) {
	return m->len == 4;
}

int send_bit(unsigned char bit) {
	volatile unsigned long long i;
	unsigned long start, stop;

	switch(bit) {
	case 0:
		usleep(TASK_TIME);
		break;
	case 1:
		start = current_time();
		for (i = 0; i < 1000000000ULL; ++i);
		stop = current_time();
		if ((stop - start) < TASK_TIME) {
			usleep(TASK_TIME - (stop - start));
		}
		break;
	default:
		return -1;
	}

	return 0;
}

int send(struct message *m) {
	const unsigned char *bytes = (const unsigned char *)m;
	const size_t bytes_size = sizeof(struct message);

	for (int i = 0; i < bytes_size; ++i) {
		unsigned char B = bytes[i];

		for (int j = 0; j < 8; ++j) {
			int ret = send_bit((B >> j) & 1);
			if (ret != 0) {
				printf("ERROR: Sender failed to send frame %u\n",
						m->frame_id);
			}
		}
	}
	return 0;
}

int recv(struct message *m) {
	return 0;
}

void send_message(struct message *m) {
	bool acked = false;
	while (!acked) {
		send(m);

		struct message r;
		recv(&r);
		acked = check_ack(&r);
	}
}

void create_message(struct message *m, const char *payload,
		unsigned int len, unsigned int frame_id) {
	memset((unsigned char *)m, 0, TOTAL_MSG_SIZE);
	m->len = len;
	m->frame_id = frame_id;
	memcpy(m->payload, payload, len);
	m->crc32 = xcrc32(m->payload, m->len, 0x0);
}

void send_ack(bool correct) {
	struct message m;
	if (correct) {
		//create_message(&m, "ACK", 4);
	} else {
		//create_message(&m, "NACK", 5);
	}
	send(&m);
}

bool check_crc32(struct message *m) {
	unsigned int computed_crc32 = xcrc32(m->payload, m->len, 0x0);
	return computed_crc32 == m->crc32;
}

void recv_message(struct message *m) {
	recv(m);
	bool correct = check_crc32(m);
	send_ack(correct);
}

unsigned long current_time() {
	struct timeval tv;
	gettimeofday(&tv,NULL);

	unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
	return time_in_micros;
}

void consume_time(unsigned long end) {
	unsigned long start = current_time();
	unsigned long diff = end - start;

	usleep(diff);
}
