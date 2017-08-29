#include "utils.h"

#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>

extern unsigned int xcrc32(const unsigned char *, int, unsigned int);
unsigned long current_time();

bool check_ack(struct message *m) {
	return (strncmp(m->payload, "NACK", 4) == 0);
}

int send_bit(unsigned char bit) {
	volatile unsigned long long i;
	unsigned long start, stop, diff;

	switch(bit) {
	case 0:
		usleep(SEND_TIME);
		break;
	case 1:
		start = current_time();
		for (i = 0; i < OPS; ++i);
		stop = current_time();

		diff = stop - start;
		if (diff < SEND_TIME) {
			usleep(SEND_TIME - diff);
			printf("INFO: Sender slept for: %lu (us)\n", SEND_TIME - diff);
		}
		break;
	default:
		return -1;
	}

	return 0;
}

int recv_bit() {
	volatile unsigned long long i;
	unsigned long start, stop, diff;

	start = current_time();
	for (i = 0; i < OPS; ++i);
	stop = current_time();

	diff = stop - start;
	if (diff < TASK_TIME) {
		usleep(TASK_TIME - diff);
		printf("INFO: Receiver slept for: %lu (us)\n", TASK_TIME - diff);
	}

	if (diff > TASK_TIME)
		return 1;
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
				return -1;
			}
		}
		if (i % SYNC_BYTES == 0) {
			printf("INFO: Sent: %.2f of msg at: %lu\n",
					(i * 1.0 / bytes_size * 1.0) * 100,
					current_time() / 1000000L);
		}
	}
	return 0;
}

int recv(struct message *m) {
	unsigned char *bytes = (unsigned char *)m;
	size_t bytes_size = sizeof(struct message);

	memset(bytes, 0, bytes_size);
	for (int i = 0; i < bytes_size; ++i) {
		for (int j = 0; j < 8; ++j) {
			int bit = recv_bit();
			if (bit) {
				bytes[i] |= 1 << j;
			}
		}
		if (i % SYNC_BYTES == 0) {
			printf("INFO: Received: %.2f%% of msg at: %lu\n",
					(i * 1.0 / bytes_size * 1.0) * 100.0,
					current_time() / 1000000L);
		}
	}
	printf("INFO: Recv frame: %u with len %u\n", m->frame_id, m->len);
	return 0;
}

void send_message(struct message *m) {
	bool acked = false;
	printf("INFO: Sending frame: %u len: %u\n", m->frame_id, m->len);
	while (!acked) {
		send(m);

		struct message r;
		recv(&r);
		acked = check_ack(&r);
		if (!acked) {
			printf("WARN: Ack not received\n");
		}
	}
	printf("INFO: Sent frame: %u\n", m->frame_id);
}

void create_message(struct message *m, const char *payload,
		unsigned int len, unsigned int frame_id) {
	memset((unsigned char *)m, 0, TOTAL_MSG_SIZE);
	m->len = len;
	m->frame_id = frame_id;
	memcpy(m->payload, payload, len);
	m->crc32 = xcrc32(m->payload, m->len, 0x0);
}

void send_ack(bool correct, unsigned int frame_id) {
	struct message m;
	if (correct) {
		create_message(&m, "ACK", 4, frame_id);
	} else {
		create_message(&m, "NACK", 5, frame_id);
	}
	send(&m);
}

bool check_crc32(struct message *m) {
	if (m->len > MAX_MSG_CHARS)
		return false;
	unsigned int computed_crc32 = xcrc32(m->payload, m->len, 0x0);
	return computed_crc32 == m->crc32;
}

void recv_message(struct message *m) {
	bool correct = false;

	printf("INFO: Receiving message\n");
	while (!correct) {
		recv(m);
		correct = check_crc32(m);
		send_ack(correct, m->frame_id);
	}
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

unsigned long get_rr_time_quantum() {
	struct timespec t;
	pid_t pid = getpid();

	int ret = sched_rr_get_interval(pid, &t);
	return (unsigned long)((t.tv_sec * 1000000000ULL + t.tv_nsec) / 1000);
}
