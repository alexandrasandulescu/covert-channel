objects = utils.o crc32.o
current_dir = $(shell pwd)

CFLAGS = -g
LDFLAGS = -L/home/alex/covert-channel
LINK = -lutils

all: libutils.a sender receiver

sender: sender.o
	$(CC) $(CFLAGS) sender.o $(LDFLAGS) -o $@ $(LINK)

receiver: receiver.o
	$(CC) $(CFLAGS) receiver.o $(LDFLAGS) -o $@ $(LINK)

libutils.a: $(objects)
	$(AR) rcs $@ $^

%.o:%.c
	$(CC) -g -c $<

.PHONY:clean
clean:
	$(RM) $(objects) sender.o receiver.o *.a sender receiver
