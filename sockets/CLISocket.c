// Socket.c

#include "ISocket.c"

// Define the Socket structure that implements ISocket
typedef struct {
    ISocket base;
    // Add any specific members for the Socket
    // ...
} Socket;

// Implementation of ISocket methods for Socket
static int Socket_open(void *self) {
    // Implementation for opening a socket
    printf("Opening client socket cli.");
    return 0;
}

static int Socket_close(void *self) {
    // Implementation for closing a socket
    return 0;
}

static int Socket_send(void *self, const char *data, size_t length) {
    // Implementation for sending data through a socket
    return 0;
}

static int Socket_receive(void *self, char *buffer, size_t length) {
    // Implementation for receiving data from a socket
    return 0;
}

// Initialize the Socket structure with ISocket methods
CLISocket cliSocketInstance = {
    .base = {
        .open = Socket_open,
        .close = Socket_close,
        .send = Socket_send,
        .receive = Socket_receive,
    };

    return socket;
}
