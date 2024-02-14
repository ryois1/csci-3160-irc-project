CC=gcc
CFLAGS=-Wall
PTHREAD=-pthread

all: server client

server: server.c message.c
	$(CC) $(CFLAGS) -o server server.c message.c

client: client.c
	$(CC) $(CFLAGS) $(PTHREAD) -o client client.c

clean:
	rm -f server client