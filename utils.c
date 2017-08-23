#include "utils.h"

#include <stdbool.h>
#include <string.h>

bool check_ack(struct message *m) {
	return m->len == 4;
}

int send(struct message *m) {
	return 0;
}

int recv(struct message *m) {
	return 0;
}

void send_msg(struct message *m) {
	bool acked = false;
	while (!acked) {
		send(m);

		struct message r;
		recv(&r);
		acked = check_ack(&r);
	}
}

void create_message(struct message *m, const char *payload, unsigned int len) {
	memset((unsigned char *)m, 0, TOTAL_MSG_SIZE);
	m->len = len;
	memcpy(m->payload, payload, len);
}

void send_ack(bool correct) {
	struct message m;
	if (correct) {
		create_message(&m, "ACK", 4);
	} else {
		create_message(&m, "NACK", 5);
	}
	send(&m);
}

extern unsigned int xcrc32(const unsigned char *, int, unsigned int);

bool check_crc32(struct message *m) {
	unsigned int computed_crc32 = xcrc32(m->payload, m->len, 0x0);
	return computed_crc32 == m->crc32;
}

void recv_msg(struct message *m) {
	recv(m);
	bool correct = check_crc32(m);
	send_ack(correct);
}
