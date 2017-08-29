#ifndef __UTILS__
#define __UTILS__

#define TOTAL_MSG_SIZE 64
#define MAX_MSG_SIZE ((TOTAL_MSG_SIZE) - sizeof(unsigned int) - \
		sizeof(unsigned int) - sizeof(unsigned int))
#define MAX_MSG_CHARS 16
#define SYNC_BYTES 16

#define TASK_TIME 16000
#define SEND_TIME ((TASK_TIME) + (TASK_TIME) / 2)

#define OPS 9000000UL

struct message {
	unsigned int len;
	char payload[MAX_MSG_SIZE];
	unsigned int crc32;
	unsigned int frame_id;
};

void consume_time(unsigned long);
unsigned long get_rr_time_quantum();
void create_message(struct message *, const char *, unsigned int, unsigned int);
void send_message(struct message *);
void recv_message(struct message *m);

#endif
