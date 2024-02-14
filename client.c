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

void *receive_message(void *sockfd_ptr)
{
    int sock = *(int *)sockfd_ptr; // Cast the socket file descriptor to the correct type

    while (1)
    {
        uint32_t msg_length_net, msg_length;
        // Read the message length first
        int read_status = read(sock, &msg_length_net, sizeof(msg_length_net));
        if (read_status <= 0)
        {
            printf("\nDisconnected from server.\n");
            close(sock);
            exit(EXIT_SUCCESS);
        }

        // Convert from network byte order to host byte order
        msg_length = ntohl(msg_length_net);

        if (msg_length >= BUFFER_SIZE)
        {
            // Error: Message too long, continue to listen for next message
            fprintf(stderr, "Error: Received message exceeds buffer size.\n");
            continue;
        }

        char buffer[BUFFER_SIZE] = {0};
        read_status = read(sock, buffer, msg_length);
        if (read_status > 0)
        {
            buffer[read_status] = '\0'; // Ensure string is null-terminated

            // Attempt to parse the received message
            char timestamp[20 + 1];    // Buffer for timestamp
            char user[USERNAME_LEN];   // Buffer for username
            char message[BUFFER_SIZE]; // Buffer for message

            // Parse the message assuming format: "timestamp=YYYY-MM-DD HH:MM:SS;user=username;message=text"
            if (sscanf(buffer, "timestamp=%20[^;];user=%31[^;];message=%1023[^\n]", timestamp, user, message) == 3)
            {
                // Print the formatted message if parsing is successful
                printf("\n[%s] %s: %s\n", timestamp, user, message);
            }
            else
            {
                // If parsing fails, print the raw message
                printf("\nFailed to parse message: %s\n", buffer);
            }
            // Print the prompt again
            printf("%s> ", username);
            fflush(stdout);
        }
        else
        {
            // Handle read error or disconnect
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
    }
    return NULL; // Should never reach here
}

void send_message(const char *message_content)
{
    // Get the current timestamp
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[20]; // Enough to hold "[YYYY-MM-DD HH:MM:SS]"

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Format the message
    char formatted_message[BUFFER_SIZE];
    snprintf(formatted_message, BUFFER_SIZE, "timestamp=%s;user=%s;message=%s", timestamp, username, message_content);

    // Send the formatted message as before
    uint32_t msg_length = strlen(formatted_message);
    uint32_t net_msg_length = htonl(msg_length); // Convert to network byte order

    write(sock, &net_msg_length, sizeof(net_msg_length)); // Send message length first
    write(sock, formatted_message, msg_length);           // Then send the message
    printf("Sent message to server: %s\n", formatted_message);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <server_address> <server_port> <client_username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(username, argv[3], USERNAME_LEN);
    username[USERNAME_LEN - 1] = '\0'; // Ensure null termination

    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to the server as %s\n", username);

    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_message, (void *)&sock) != 0)
    {
        perror("pthread_create failed");
        return EXIT_FAILURE;
    }

    char message[BUFFER_SIZE];
    while (1)
    {
        printf("%s> ", username); // Show the username as part of the prompt
        fflush(stdout);
        if (fgets(message, BUFFER_SIZE, stdin) != NULL)
        {
            send_message(message);
        }
    }

    close(sock);
    return 0;
}
