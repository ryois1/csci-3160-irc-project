#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "ISocket.c"

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
    connect(clisocket->sfd, result->ai_addr, result->ai_addrlen);
    if (clisocket->sfd == -1) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Free the address information since it's no longer needed
    freeaddrinfo(result);

    // Successfully connected to the server
    //TODO determine how to fork properly so that the original program can continue or even ask the user for inputs while also allowing the socket to receive messages.

    //TODO FORK call socket_recieve
    char             buf[32];
    Socket_receive(clisocket,buf)
    
    close(clisocket->sfd);
    return 0;
}

static int Socket_close(void *self) {
    // Implementation for closing a socket
    int              bytes;
    
}

static int Socket_send(void *self, const char *data, size_t length) {
    // Implementation for sending data through a socket
    return 0;
}

static int Socket_receive(void *self, char *buffer, size_t length) {
    // Implementation for receiving data from a socket 
    int              bytes;
    ssize_t          nread;
    CLISocket *clisocket = (CLISocket *)self;
    //TODO pthread lock unlock so its not taking up all the time?
    while(1){
        printf("Client writing ping\n");
        bytes = write(clisocket->sfd, "ping", 5); // Send "ping" to the server
        //TODO fork into read loop.
        bytes = read(clisocket->sfd, buf, 5);       // Read the response from the server
        printf("PID: %d; client received %s\n", getpid(), buf);

        
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
