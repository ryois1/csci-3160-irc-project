// ISocket.h


#ifndef ISOCKET_H
#define ISOCKET_H

// Define the ISocket interface
typedef struct {
    // Method to open the socket
    int (*open)(void *self);

    // Method to close the socket
    int (*close)(void *self);

    // Method to send data through the socket
    int (*send)(void *self, const char *data, size_t length);

    // Method to receive data from the socket
    int (*receive)(void *self);
} ISocket;

#endif /* ISOCKET_H */
