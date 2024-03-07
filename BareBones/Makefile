CC=gcc
CFLAGS=-Wall
PTHREAD=-pthread

all: server client

server: server.c
	$(CC) $(CFLAGS) $(PTHREAD) -o server bserver.c 
client: client.c
	$(CC) $(CFLAGS) $(PTHREAD) -o client bclient.c 

clean:
	rm -f server client