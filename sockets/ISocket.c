// ISocket.h

#ifndef ISOCKET_H
#define ISOCKET_H

// Define the ISocket interface
typedef struct {
    // Method to open the socket
    int (*open)(void);

    // Method to close the socket
    int (*close)(void *self);

    // Method to send data through the socket
    int (*send)(void *self, const char *data, size_t length);

    // Method to receive data from the socket
    int (*receive)(void *self, char *buffer, size_t length);
} ISocket;

#endif /* ISOCKET_H */
