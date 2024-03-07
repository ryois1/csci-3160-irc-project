#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>

#include "ISocket.h"

// Define the Socket structure that implements ISocket
typedef struct {
    ISocket base;
    // Add any specific members for the Socket
    // ...
    int sfd;
} CLISocket;

// Implementation of ISocket methods for Socket
int Socket_open(void *self) {
    CLISocket *clisocket = (CLISocket *)self;
    // Implementation for opening a socket
    printf("Opening client socket cli.\n");
    // Variables
    int              bytes;
    char             buf[32];
    ssize_t          nread;
    struct addrinfo  hints;
    struct addrinfo  *result;

    /* Obtain address matching host/port. */

    // Initialize address hints structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_TCP; /* TCP */

    // Get the address from the variables
    if (getaddrinfo("127.0.0.1","9000", &hints, &result) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Create a socket
    clisocket->sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    int connectresult = connect(clisocket->sfd, result->ai_addr, result->ai_addrlen);
    if (connectresult == -1) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Free the address information since it's no longer needed
    freeaddrinfo(result);

    // Successfully connected to the server
    //TODO determine how to fork properly so that the original program can continue or even ask the user for inputs while also allowing the socket to receive messages.
    printf("Successfully connected!!\n");
    

    return 0;
}

static int Socket_close(void *self) {
    // Implementation for closing a socket
    if (close(((CLISocket *)self)->sfd) == -1) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static int Socket_send(void *self, const char *data, size_t length) {
    // Implementation for sending data through a socket
    int bytes;
    CLISocket *clisocket = (CLISocket *)self;
    //if send is empty, return 0
    if (length == 0) {
        return 0;
    }
    printf("PRE Write : %s\n",data);

    bytes = write(clisocket->sfd, data, length);
    printf("Bytes written %d \n",bytes);\
    fprintf(stderr, "ERROR? : %s\n", strerror(errno));
    return bytes;

}

static int Socket_receive(void *self) {
    int              bytes;
    ssize_t          nread;
    CLISocket *clisocket = (CLISocket *)self;
    //TODO pthread lock unlock so its not taking up all the time?
    while(true){
        //TODO fork into read loop.
        //pthread lock then unlock after finishing read
        char             buf[32];
        bytes = read(clisocket->sfd, buf, 32);       // Read the response from the server

        if(bytes>0){
        printf("PID: %d; client received %s\n", getpid(), buf);
        }
        memset(buf,'\0',32);
        // Sleep for a short duration to avoid busy-waiting
        usleep(1000000);  // milliseconds
        }
        return 0;
    }


// Initialize the Socket structure with ISocket methods
CLISocket cliSocketInstance = {
    .base = {
        .open = Socket_open,
        .close = Socket_close,
        .send = Socket_send,
        .receive = Socket_receive,
    }
};
