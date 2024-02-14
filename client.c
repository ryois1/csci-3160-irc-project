#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "common.h"

char username[USERNAME_LEN];
int sock; // Socket descriptor

void *receive_message(void *sockfd) {
    char buffer[BUFFER_SIZE] = {0};

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int read_status = read(sock, buffer, BUFFER_SIZE - 1);
        if (read_status > 0) {
            // Print the message directly received from the server
            printf("\r%s\n%s> ", buffer, username);
            fflush(stdout);
        } else if (read_status == 0) {
            printf("\nDisconnected from server.\n");
            close(sock);
            exit(EXIT_SUCCESS);
        } else {
            perror("recv");
            exit(EXIT_FAILURE);
        }
    }
}

void send_message(const char *message) {
    if (strlen(message) > 1) { // Ensuring the message is more than a newline
        char buffer[BUFFER_SIZE];
        snprintf(buffer, sizeof(buffer), "%s: %s", username, message);
        write(sock, buffer, strlen(buffer));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_address> <server_port> <client_username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(username, argv[3], USERNAME_LEN);
    username[USERNAME_LEN - 1] = '\0'; // Ensure null termination

    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to the server as %s\n", username);

    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_message, NULL) != 0) {
        perror("pthread_create failed");
        return EXIT_FAILURE;
    }

    char message[BUFFER_SIZE];
    while (1) {
        printf("%s> ", username); // Show the username as part of the prompt
        fflush(stdout);
        if (fgets(message, BUFFER_SIZE, stdin) != NULL) {
            send_message(message);
        }
    }

    close(sock);
    return 0;
}
