// IServerSocket.h

#ifndef ISERVERSOCKET_H
#define ISERVERSOCKET_H

// Define the IServerSocket interface
typedef struct {
    // Method to initialize the server socket
    void (*initialize)(void);

    // Method to bind the server socket to a specific address and port
    int (*bind)(const char* address, int port);

    // Method to listen for incoming connections on the server socket
    int (*listen)(int backlog);

    // Method to listen for incoming messages
    int (*onRecieve)();

    // Method to accept a new incoming connection and return the client socket
    int (*acceptConnection)(void);

    // Method to clean up resources associated with the server socket
    void (*cleanup)(void);
} IServerSocket;

#endif // ISERVERSOCKET_H
