#ifndef __UTILS__
#define __UTILS__

#define TOTAL_MSG_SIZE 1024
#define MAX_MSG_SIZE ((TOTAL_MSG_SIZE) - sizeof(unsigned int) - \
		sizeof(unsigned int))
#define MAX_MSG_CHARS 10

struct message {
	unsigned int len;
	char payload[MAX_MSG_SIZE];
	unsigned int crc32;
};

#endif
