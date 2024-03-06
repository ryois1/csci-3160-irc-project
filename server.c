#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


#include "servers/IServerSocket.h"
#include "servers/WebServer.c"
#include "servers/CLIServer.c"

int global_connections_over_socket = 0;

int main() {
    //Clears the screen because fuck making a standardized clear screen method cross platform :)
    printf("\e[1;1H\e[2J");

    IServerSocket webserver = WebServerInstance.base;
    IServerSocket cliserver = cliServerInstance.base;

    //TODO
    //Add variable called messages that both servers have access too, it should be atomic (think our most recent lab)
    //Add a list of ISocket, that both have access to, then when one recieves a message it can simply go through the list and forward the message to the other sockets.

    cliserver.initialize(&global_connections_over_socket);
    cliserver.bind("localhost",8888);



    webserver.initialize();
    webserver.bind("localhost",80);

    // Create a string to store server user input
    char command[32];

    //While the user input is not "exit", continue asking for input.
    while(strcmp(command,"exit")!=0){
        printf("\x1B[34m   CLI: \033[0m");
        scanf("%s", command);
        //TODO add other commands by using strcmp,
        //or by creating another file with a method called processCommand();
    }



    //Close all ports and do any needed freeing of memory here.
    //Closing ports ensures the program can be run shortly after.
    cliserver.cleanup();
    webserver.cleanup();

    return 0;
}