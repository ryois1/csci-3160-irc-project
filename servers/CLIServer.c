// CLIServer.c
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/select.h> 
#include <stdatomic.h>

int connections[32];

// Define the CLIServer class
typedef struct {
    IServerSocket base; // Inherit from IServerSocket interface
    // Add additional members specific to CLIServer if needed
    // ...
} CLIServer;

// Function to initialize the CLIServer
static void initializeCLIServer() {

        // Create a shared memory region
    int *connection_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (connection_count == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize the shared variable
    *connection_count = 0;
    
    // Initialization logic specific to CLIServer
    printf("\x1B[32m   Initializing CLI Server... \n\033[0m");

    //Declare variables
    int sfd;
    char buf[32];
    char bufRec[32];
    struct addrinfo hints, *result, *rp;
    const char *portnum = "9000";
    int reuse_port = 1;

    

    //Declare more variables
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */


    //Get the address
    int s = getaddrinfo(NULL, portnum, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* Create socket and set SO_REUSEPORT option */
    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(sfd, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* No longer needed */
    freeaddrinfo(result);           


	/* Listen to the gorram socket */
	if (listen(sfd, 1) < 0) {
		fprintf(stderr, "Error in listen: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
    //TODO fork this so that the server can continue to listen for connections while also allowing the user to input commands.
    pid_t child_pid = fork();
    if(child_pid == 0){
        int connectioncountlocal = 10;
        while(1){
            int connection;
            printf("Child process created\n");
            printf("Calling accept\n");
            connection = accept(sfd, NULL, NULL);
            if (connection < 0) {
                fprintf(stderr, "Error in accept: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            printf("Accepted %d\n ",connection);
            //connections[connection_count] = connection;
            *connection_count = *connection_count  + 1;
            
            printf("Connected count amount: %d \n",*connection_count);
        }
    }
	

    while(1){
       //* Do the ping-pong thing */
       //for each connection do this:
       printf("Connected: %d \n",*connection_count);
       usleep(1000000);  // milliseconds

         for(int i = 0; i < *connection_count; i++){
            //  fcntl(connections[i], F_SETFL, SOCK_NONBLOCK); //non blocking read
            //  read(connections[i], bufRec, 32);
            //  printf("PID: %d; server received %s\n", getpid(), bufRec);
            //  read(connection, buf, 5);
            //  printf("PID: %d; server received %s\n", getpid(), buf);
            // strcpy(buf, "pong");
            // printf("Server writes %s\n", buf);
            // write(connections[i], buf, 5);

            // Add If statement here, if the message is new.
            for(int j = 0; j < *connection_count; j++){
                if(connections[i]==connections[j]){continue;}
                //send message we just read back out to every other connection
                //write(connections[j], , );
            }


        }

    }
    free(connections);

	//close(connection);			/* Tear down the session with client */
}

// Function to bind the CLIServer to a specific address and port
static int bindCLIServer(const char* address, int port) {
    // Binding logic specific to CLIServer
    printf("\x1B[32m   Binding CLI Server to %s:%d... \n\033[0m", address, port);
    // Implement actual binding logic here
    return 0; // Return 0 for success, -1 for failure
}

// Function to listen for incoming connections on the CLIServer
static int listenCLIServer(int backlog) {
    // Listening logic specific to CLIServer
    printf("CLI Server listening for connections...\n");
    // Implement actual listening logic here
    return 0; // Return 0 for success, -1 for failure
}

// Function to accept a new incoming connection and return the client socket
static int acceptConnectionCLIServer(void) {
    // Accept connection logic specific to CLIServer
    printf("CLI Server accepting connection...\n");
    // Implement actual connection acceptance logic here
    return 123; // Replace with actual client socket value
}

// Function to clean up resources associated with the CLIServer
static void cleanupCLIServer(void) {
    // Cleanup logic specific to CLIServer
    printf("\x1B[31m   Cleaning up CLI Server...   \n\033[0m");
    // Implement actual cleanup logic here
}

static void cliRecieve(void){
    printf("\x1B[36m CLI Recieved Message  \n\033[0m");
    printf("Looping through sockets, sending to every other socket.\n");
}

// Create an instance of CLIServer and set function pointers
CLIServer cliServerInstance = {
    .base = {
        .initialize = initializeCLIServer,
        .bind = bindCLIServer,
        .listen = listenCLIServer,
        .acceptConnection = acceptConnectionCLIServer,
        .cleanup = cleanupCLIServer,
        .onRecieve = cliRecieve
    }
};