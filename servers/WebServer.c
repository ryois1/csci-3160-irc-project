// WebServer.c

#include <stdio.h>
#include <stdlib.h>

// Define the WebServer class
typedef struct {
    IServerSocket base; // Inherit from IServerSocket interface
    // Add additional members specific to WebServer if needed
    // ...
} WebServer;

// Function to initialize the WebServer
static void initializeWebServer(void) {
    // Initialization logic specific to WebServer
    //Green
    printf("\x1B[32m   Initializing Web Server... \n\033[0m");
}

// Function to bind the WebServer to a specific address and port
static int bindWebServer(const char* address, int port) {
    // Binding logic specific to WebServer
    //Green
    printf("\x1B[32m   Binding Web Server to %s:%d... \n\033[0m", address, port);
    // Implement actual binding logic here
    return 0; // Return 0 for success, -1 for failure
}

// Function to listen for incoming connections on the WebServer
static int listenWebServer(int backlog) {
    // Listening logic specific to WebServer
    printf("Web Server listening for connections...\n");
    // Implement actual listening logic here
    return 0; // Return 0 for success, -1 for failure
}

// Function to accept a new incoming connection and return the Webent socket
static int acceptConnectionWebServer(void) {
    // Accept connection logic specific to WebServer
    printf("Web Server accepting connection...\n");
    // Implement actual connection acceptance logic here
    return 123; // Replace with actual id.
}

// Function to clean up resources associated with the WebServer
static void cleanupWebServer(void) {
    // Cleanup logic specific to WebServer
    printf("\x1B[31m   Cleaning up Web Server...   \n\033[0m");
    // Implement actual cleanup logic here
}

void recieve(void){
    printf("\x1B[36m Web Recieved Message  \n\033[0m");
    printf("Looping through sockets, sending to every other socket.\n");
}

// Create an instance of WebServer and set function pointers
WebServer WebServerInstance = {
    .base = {
        .initialize = initializeWebServer,
        .bind = bindWebServer,
        .listen = listenWebServer,
        .acceptConnection = acceptConnectionWebServer,
        .cleanup = cleanupWebServer,
        .onRecieve = recieve
    }
};