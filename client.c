#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "Common.h"
#include "messages/Message.h"

char username[USERNAME_LEN];
int sock; // Socket descriptor

void *receive_message(void *sockfd_ptr)
{
    int sock = *(int *)sockfd_ptr;
    while (1)
    {
        uint32_t msg_length_net, msg_length;
        // Initial read for the message length
        if (read(sock, &msg_length_net, sizeof(msg_length_net)) <= 0)
        {
            printf("\nDisconnected from server.\n");
            close(sock);
            exit(EXIT_SUCCESS);
        }
        msg_length = ntohl(msg_length_net);

        if (msg_length >= BUFFER_SIZE)
        {
            continue;
        }

        char buffer[BUFFER_SIZE] = {0};
        size_t total_read = 0;
        while (total_read < msg_length)
        {
            ssize_t read_status = read(sock, buffer + total_read, msg_length - total_read);
            if (read_status > 0)
            {
                total_read += read_status;
            }
            else
            {
                // Handle read error or disconnect
                perror("Receive failed");
                exit(EXIT_FAILURE);
            }
        }
        buffer[msg_length] = '\0'; // Ensure null-terminated

        Message msg;
        deserialize_message(buffer, &msg); // Use the deserialize function

        // Print the parsed message
        printf("\n[%s] %s: %s\n", msg.timestamp, msg.user, msg.content);
        printf("%s> ", username);
        fflush(stdout);
    }
    return NULL;
}

void send_message(const char *message_content)
{
    Message msg;
    create_message(&msg, username, message_content); // Create a message struct
    char *serialized_msg = serialize_message(&msg);  // Serialize the message

    uint32_t msg_length = strlen(serialized_msg);
    uint32_t net_msg_length = htonl(msg_length);

    write(sock, &net_msg_length, sizeof(net_msg_length));
    write(sock, serialized_msg, msg_length);

    free_serialized_message(serialized_msg); // Free the allocated serialized message
}

int connect_to_server(const char *server_address, const char *server_port)
{
    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    if (inet_pton(AF_INET, server_address, &server_addr.sin_addr) <= 0)
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

    return 0;
}

void close_connection()
{
    close(sock);
    exit(EXIT_SUCCESS);
}



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
