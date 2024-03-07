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
#include <sys/mman.h>

int connection_count = 0;
int connections[32];
int serverstart();
void acceptconnect(int);
void processmessages();

int main() {
    int server = serverstart();

    while(1){
        acceptconnect(server);
        processmessages();
    }
}

int serverstart(){
    // Initialization logic specific to CLIServer
    printf("\x1B[32m   Initializing CLI Server... \n\033[0m");

    //Declare variables
    int sfd;
    struct addrinfo hints, *result, *rp;
    const char *portnum = "9000";
    int reuse_port = 1;

    

    //Declare more variables
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
    hints.ai_protocol = IPPROTO_TCP;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    //Get the address
    int s = getaddrinfo(NULL, portnum, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* Create socket and set SO_REUSEPORT option */
    sfd = socket(result->ai_family, result->ai_socktype | SOCK_NONBLOCK, result->ai_protocol);
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
    return sfd;
}

void acceptconnect(int file){
    int connection = accept(file, NULL, NULL);

    if (connection >0) {
        printf("Accepted %d\n", connection);
        connections[connection_count] = connection;
        ++connection_count;
        printf("Connected count amount: %d\n", connection_count);
    }
}

void processmessages(){
    char bufRec[32];
    int i = 0;
    for(; i < connection_count; i++){
        //Read message
        fcntl(connections[i], F_SETFL, SOCK_NONBLOCK); //non blocking read
        int reading = read(connections[i], bufRec, 32);
        if(reading<=0)continue;
        printf("Relaying message for: %d\n",connections[i]);
        
        //Relay Message
        int j=0;
        for(; j < connection_count; j++){
            if((int)connections[i]==(int)connections[j]){continue;}
            printf("Writing to %d\n",connections[j]);
            int byteswritten = write(connections[j], bufRec, 32);
            if(byteswritten==-1){
                //welp so long fellas the program breaks.
            }
            printf("Bytes written: %d\n",byteswritten);
        }
        //Clear buffer.
        memset(bufRec,"\0",32);
    }
}

