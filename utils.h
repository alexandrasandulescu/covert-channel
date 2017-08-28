#ifndef __UTILS__
#define __UTILS__

#define TOTAL_MSG_SIZE 1024
#define MAX_MSG_SIZE ((TOTAL_MSG_SIZE) - sizeof(unsigned int) - \
		sizeof(unsigned int) - sizeof(unsigned int))
#define MAX_MSG_CHARS 10

#define TASK_TIME 1000000

struct message {
	unsigned int len;
	char payload[MAX_MSG_SIZE];
	unsigned int crc32;
	unsigned int frame_id;
};

void consume_time(unsigned long);
void create_message(struct message *, const char *, unsigned int, unsigned int);
void send_message(struct message *);

#endif
