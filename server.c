#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


#include "servers/IServerSocket.h"
#include "servers/WebServer.c"
#include "servers/CLIServer.c"



int main() {
    //Clears the screen because fuck making a standardized clear screen method cross platform :)
    printf("\e[1;1H\e[2J");

    IServerSocket webserver = WebServerInstance.base;
    IServerSocket cliserver = cliServerInstance.base;

    //TODO
    //Add variable called messages that both servers have access too, it should be atomic (think our most recent lab)
    //Add a list of ISocket, that both have access to, then when one recieves a message it can simply go through the list and forward the message to the other sockets.

    cliserver.initialize();
    cliserver.bind("localhost",8888);



    webserver.initialize();
    webserver.bind("localhost",80);

    // Create a string
    char command[32];
    
    //While the user input is not "exit", continue asking for input.
    while(strcmp(command,"exit")!=0){
        printf("CLI:");
        scanf("%s", command);
    }




    cliserver.cleanup();
    webserver.cleanup();

    return 0;
}