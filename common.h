// common.h
#ifndef COMMON_H
#define COMMON_H

#define MAX_CLIENTS 30
#define BUFFER_SIZE 1025
#define USERNAME_LEN 32
#define SERVER_PORT 8888

typedef struct {
    int sockfd;
    char username[USERNAME_LEN];
} User;

typedef struct {
    int sockfd;
    char username[USERNAME_LEN];
} client_t;

#endif
