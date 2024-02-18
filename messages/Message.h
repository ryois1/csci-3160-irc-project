// message.h
#ifndef MESSAGE_H
#define MESSAGE_H
#include "common.h"

typedef struct {
    char timestamp[20];  // YYYY-MM-DD HH:MM:SS
    char user[USERNAME_LEN];
    char content[BUFFER_SIZE];
} Message;

void create_message(Message* msg, const char* user, const char* content);
char* serialize_message(const Message* msg);
void deserialize_message(const char* msg_str, Message* msg);
void free_serialized_message(char* msg);

#endif