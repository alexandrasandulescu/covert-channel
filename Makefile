objects := utils.o crc32.o

all: $(objects) sender receiver

sender: sender.o
	$(CC) $(CFLAGS) $(objects) sender.c -o $@

receiver: receiver.o
	$(CC) $(CFLAGS) $(objects) receiver.c -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	$(RM) $(objects) sender.o receiver.o sender receiver
