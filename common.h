// common.h
#ifndef COMMON_H
#define COMMON_H

#define MAX_USERS 10
#define BUFFER_SIZE 1024
#define USERNAME_LEN 32
#define SERVER_PORT 8080

typedef struct {
    int sockfd;
    char username[USERNAME_LEN];
} User;

#endif
