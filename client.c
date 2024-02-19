#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "sockets/CLISocket.c"


int main(int argumentCount, char *arguments[])
{
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


    //For now just print out what arguments the user ended up with.
    for (int i = 0; i < argumentCount; i++) {
        printf("Arg %s %d \n",arguments[i],i);
    }

    CLISocket socket = Socket_create();
    socket.open();

    return 0;
}
