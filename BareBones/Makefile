CC=gcc
CFLAGS=-Wall
PTHREAD=-pthread

all: server client

server: server.c
	$(CC) $(CFLAGS) $(PTHREAD) -o server server.c 
client: client.c
	$(CC) $(CFLAGS) $(PTHREAD) -o client client.c 

clean:
	rm -f server client