CC=gcc
CFLAGS=-Wall
PTHREAD=-pthread

all: server client

server: bserver.c
	$(CC) $(CFLAGS) $(PTHREAD) -o server bserver.c 
client: bclient.c
	$(CC) $(CFLAGS) $(PTHREAD) -o client bclient.c 

clean:
	rm -f server client