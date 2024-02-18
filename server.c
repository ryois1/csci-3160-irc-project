#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


#include "servers/IServerSocket.h"
#include "servers/WebServer.c"
#include "servers/CLIServer.c"



int main() {
    IServerSocket webserver = WebServerInstance.base;
    IServerSocket cliserver = CLIServerInstance.base;

    //TODO
    //Add variable called messages that both servers have access too, it should be atomic (think our most recent lab)
    //Add a list of ISocket, that both have access to, then when one recieves a message it can simply go through the list and forward the message to the other sockets.

    cliserver.initialize();
    cliserver.bind();
    cliserver.cleanup();


    webserver.initialize();
    webserver.bind();
    webserver.cleanup();

    return 0;
}