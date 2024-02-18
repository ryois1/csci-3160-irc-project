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
    if (argumentCount != 4)
    {
        //Prints as a red error string.
        fprintf(stderr, "\x1B[31mUsage: %s <server_address> <server_port> <client_username>\n\033[0m", arguments[0]);
        exit(EXIT_FAILURE);
    }
    return 0;
}
