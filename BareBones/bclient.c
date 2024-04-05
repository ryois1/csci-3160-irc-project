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
#define MAX_MESSAGES 20
#define BUFFER_SIZE 128
int startconnect();
void checkrecieve(int file);
void sendhello(int file);

char *messages[MAX_MESSAGES];
char *username;
bool waiting = false;
char *chatColor;
bool first = true;
int main() {
    //Clears the screen
    printf("\e[1;1H\e[2J");

    // Prompt user to enter a username
    printf("Enter your username: ");
    char temp[128]; // Temporary variable to store user input
    scanf("%s", temp);

    // Allocate memory for the username and copy the input into it
    username = malloc(strlen(temp) + 1);
    strcpy(username, temp);

// Prompt user to enter chat color
    int colorChoice;
    printf("Enter a number between 1-5 to choose chat color: \n");
    printf("1. Red\n");
    printf("2. Green\n");
    printf("3. Yellow\n");
    printf("4. Blue\n");
    printf("5. Magenta\n");
    scanf("%d", &colorChoice);

    // Set chat color based on user input
    switch (colorChoice) {
        case 1:
            chatColor = "\x1B[31m"; // Red
            break;
        case 2:
            chatColor = "\x1B[32m"; // Green
            break;
        case 3:
            chatColor = "\x1B[33m"; // Yellow
            break;
        case 4:
            chatColor = "\x1B[34m"; // Blue
            break;
        case 5:
            chatColor = "\x1B[35m"; // Magenta
            break;
        default:
            printf("Invalid color choice. Defaulting to default color.\n");
            chatColor = ""; // Default color
    }


    int file =startconnect();
    while(1){
        checkrecieve(file);
        if(!waiting){
            printf("\n%s>",chatColor);
            waiting = true;
        }

        usleep(1000000);
        // sendhello(file);
        handle_user_input(file);
    }

}



int startconnect(){
    int sfd;

    printf("Opening client socket cli.\n");
    // Variables
    int              bytes;
    char             buf[128];
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
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

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
    printf("Your ID is %d\n",getpid());
    return sfd;
}

void checkrecieve(int file){
    char bufRec[128];
    bufRec[0]='\0';
    fcntl(file, F_SETFL, SOCK_NONBLOCK); //non blocking read
    int reading = read(file, bufRec, 128);
    if(reading<=0)return;
    waiting = false;

    // Shift messages in the array to make space for the new message
    for (int i = MAX_MESSAGES - 1; i > 0; --i) {
        messages[i] = messages[i - 1];
    }

    // Allocate memory for the new message and copy it
    messages[0] = malloc(strlen(bufRec) + 1);
    strcpy(messages[0], bufRec);    
    fflush(stdout);
    printMess();

    memset(bufRec, '\0', 128);
}

void printMess(){
    printf("\e[1;1H\e[2J");
    printf("\n");
    
    // Print all messages
    for (int i = MAX_MESSAGES; i--  > 0;) {
        if (messages[i] != NULL) {
            printf("%s\n", messages[i]); // Print in reverse order
        }
    }
}

void sendhello(int file){
    char words[128];
    sprintf(words,"Hello from: %s",username);
    int byteswritten = write(file,words , 128);
    if(byteswritten < 0){
        printf("Error writing bytes.");
    }
}


void handle_user_input(int sockfd) {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int ready = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
    if (ready == -1) {
        perror("select");
    } else if (ready == 1) {

        char input[BUFFER_SIZE];
        memset(input, '\0', BUFFER_SIZE);
        fgets(input, sizeof(input), stdin);

        char message[BUFFER_SIZE];
        memset(message, '\0', BUFFER_SIZE);
        snprintf(message, sizeof(message), "%s[%s]: %s",chatColor, username, input);
        if(input[0]=="\0")return;
        if(first){
            first = false;
            return;
        }
        write(sockfd, message, strlen(message)+1);  
        waiting = false;

        for (int i = MAX_MESSAGES - 1; i > 0; --i) {
            if (messages[i - 1])
                messages[i] = strdup(messages[i - 1]);
        }

        messages[0] = strdup(message);
        printMess();
    } else {
        fflush(stdout);
    }
}

