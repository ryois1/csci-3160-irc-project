#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
int startconnect();
void checkrecieve(int file);
void sendhello(int file);
int main() {
    //Clears the screen
    printf("\e[1;1H\e[2J");

    int file =startconnect();
    while(1){
        checkrecieve(file);
        sendhello(file);
    }

}



int startconnect(){
    int sfd;

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
    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    int connectresult = connect(sfd, result->ai_addr, result->ai_addrlen);
    if (connectresult == -1) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Free the address information since it's no longer needed
    freeaddrinfo(result);

    // Successfully connected to the server
    //TODO determine how to fork properly so that the original program can continue or even ask the user for inputs while also allowing the socket to receive messages.
    printf("Successfully connected!!\n");
    return sfd;
}

void checkrecieve(int file){
    char bufRec[32];
    bufRec[0]='\0';
    fcntl(file, F_SETFL, SOCK_NONBLOCK); //non blocking read
    int reading = read(file, bufRec, 32);
    if(reading<=0)return;
    printf("received %s\n", bufRec);
}

void sendhello(int file){
    char words[32] = "hello\0";
    int byteswritten = write(file,words , 32);
    if(byteswritten <= 0){
        printf("Error writing bytes.");
    }
}