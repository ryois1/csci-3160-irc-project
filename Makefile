CC=gcc
CFLAGS=-Wall
PTHREAD=-pthread

all: server client

server: server.c Message.c
	$(CC) $(CFLAGS) $(PTHREAD) -o server server.c Message.c

client: client.c
	$(CC) $(CFLAGS) $(PTHREAD) -o client client.c Message.c

clean:
	rm -f server client