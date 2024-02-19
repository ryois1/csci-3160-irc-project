#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


int main(int argumentCount, char *arguments[])
{
    if(argumentCount>1&&argumentCount<5){
        fprintf(stderr, "\x1B[31mNot Enough arguments, using default connection.\n\033[0m");
        //Change the argument count and assign it to a new array;
        argumentCount = 5;
        free(arguments);
        arguments = (char **)malloc(sizeof(char *) * 5);
        arguments[1] = "localhost";
        arguments[2] = "8888";
        arguments[3] = "guest";
        arguments[4] = "password";
    }
    if (argumentCount != 5)
    {
        //Prints as a red error string.
        fprintf(stderr, "\x1B[31mUsage: %s <server_address> <server_port> <client_username> <client_password>\n\033[0m", arguments[0]);
        exit(EXIT_FAILURE);
    }
    return 0;
}
