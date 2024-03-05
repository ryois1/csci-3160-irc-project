#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h> /* Include boolean true/false */

#include "sockets/CLISocket.c"
#include "sockets/ISocket.h"


int main(int argumentCount, char *arguments[])
{
    //Clears the screen because fuck making a standardized clear screen method cross platform :)
    printf("\e[1;1H\e[2J");

    //Show the user proper use.
    if (argumentCount != 5)
    {
        //Prints as a red error string.
        fprintf(stderr, "\x1B[31mUsage: %s <server_address> <server_port> <client_username> <client_password>\n\033[0m", arguments[0]);
        //Optional exit the program?
        // exit(EXIT_FAILURE);
    }

    //Have backup default arguments that can be used during the showcase of the program.
    if(argumentCount<5){
        fprintf(stderr, "\x1B[31mNot Enough arguments, using default connection.\n\033[0m");
        //Change the argument count and assign it to a new array;
        argumentCount = 5;
        // free(arguments);
        arguments = (char **)malloc(sizeof(char *) * 5);
        arguments[1] = "localhost";
        arguments[2] = "8888";
        arguments[3] = "guest";
        arguments[4] = "password";
    }


    //Connect socket to server.
    ISocket socket = cliSocketInstance.base;
    socket.open(&socket);

    //Fork here, call recieve
    //create child, send child through socket.receive
    pid_t parent_pid = getpid();
    pid_t child_pid = fork();
    //if child, send through socket.receive
    if (child_pid == 0) {
        printf("pid: %d", child_pid);
        // socket.receive(&socket);
        exit(0);
    }
    
        
    char message[32];
    // //While the user input is not "exit", continue asking for their next message
    while(strcmp(message,"exit")!=0){
        //print username then a :
        printf("\x1B[34m   %s: \033[0m",arguments[3]);
        fgets(message, sizeof(message), stdin);
        printf("Msg %s",message);
        socket.send(&socket,message, 32);
    }
    exit(0);
    //TODO change params of this method so it will compile lmao.
    //socket.receive();


    //TODO move this inside the send method after we figure out the forking structure.
    // // Create a string to get user message


    //Close the connection and free resources.
    //TODO change params of this method so it will compile lmao.
    //socket.close();

    return 0;
}
