#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>
#include "common.h"

#define TRUE 1

int client_socket[MAX_CLIENTS];

// Function prototypes
void initialize_server(int *master_socket, struct sockaddr_in *address);
void accept_new_connection(int master_socket, struct sockaddr_in *address);
void handle_client_message(int client_fd);
void echo_to_all_clients(int sender_fd, char *buffer, int buf_size);

int main() {
    int master_socket, activity, i, sd;
    struct sockaddr_in address;
    fd_set readfds;

    // Initialize all client_socket[] to 0 so not checked
    memset(client_socket, 0, sizeof(client_socket));

    // Initialize the server (create and bind)
    initialize_server(&master_socket, &address);

    puts("Waiting for connections ...");

    while (TRUE) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add master socket to set
        FD_SET(master_socket, &readfds);
        int max_sd = master_socket;

        // Add child sockets to set
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // Wait for an activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }

        // Incoming connection
        if (FD_ISSET(master_socket, &readfds)) {
            accept_new_connection(master_socket, &address);
        }

        // IO operation on some other socket
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                handle_client_message(sd);
            }
        }
    }

    return 0;
}

void initialize_server(int *master_socket, struct sockaddr_in *address) {
    // Create a master socket
    if ((*master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set master socket to allow multiple connections
    int opt = 1;
    if (setsockopt(*master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Type of socket created
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(SERVER_PORT);

    // Bind the socket to localhost port 8888
    if (bind(*master_socket, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", SERVER_PORT);

    // Try to specify maximum of 3 pending connections for the master socket
    if (listen(*master_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void accept_new_connection(int master_socket, struct sockaddr_in *address) {
    int addrlen = sizeof(struct sockaddr_in);
    int new_socket;
    if ((new_socket = accept(master_socket, (struct sockaddr *)address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("New connection, socket fd is %d, ip is: %s, port: %d\n", new_socket, inet_ntoa(address->sin_addr), ntohs(address->sin_port));

    // Add new socket to array of sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_socket[i] == 0) {
            client_socket[i] = new_socket;
            printf("Adding to list of sockets as %d\n", i);
            break;
        }
    }
}

void handle_client_message(int client_fd) {
    char buffer[BUFFER_SIZE];
    int valread = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (valread == 0) {
        // Somebody disconnected, get his details and print
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        getpeername(client_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        printf("Host disconnected, ip %s, port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // Close the socket and mark as 0 in list for reuse
        close(client_fd);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_socket[i] == client_fd) {
                client_socket[i] = 0;
                break;
            }
        }
    } else {
        buffer[valread] = '\0';
        echo_to_all_clients(client_fd, buffer, valread);
    }
}

void echo_to_all_clients(int sender_fd, char *buffer, int buf_size) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_socket[i] != 0 && client_socket[i] != sender_fd) {
            send(client_socket[i], buffer, buf_size, 0);
        }
    }
}
