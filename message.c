// message.c
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"

void current_timestamp(char* ts) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(ts, 20, "%Y-%m-%d %H:%M:%S", t);
}

void create_message(Message* msg, const char* user, const char* content) {
    current_timestamp(msg->timestamp);
    strncpy(msg->user, user, sizeof(msg->user) - 1);
    msg->user[sizeof(msg->user) - 1] = '\0'; // Ensure null termination
    strncpy(msg->content, content, sizeof(msg->content) - 1);
    msg->content[sizeof(msg->content) - 1] = '\0'; // Ensure null termination
}


char* serialize_message(const Message* msg) {
    char* serialized = malloc(20 + 1 + USERNAME_LEN + 1 + BUFFER_SIZE + 1); // timestamp + user + content + extra for format
    sprintf(serialized, "timestamp=%s;user=%s;message=%s", msg->timestamp, msg->user, msg->content);
    return serialized;
}

void deserialize_message(const char* msg_str, Message* msg) {
    sscanf(msg_str, "timestamp=%20[^;];user=%31[^;];message=%1023[^\n]", msg->timestamp, msg->user, msg->content);
}

void free_serialized_message(char* msg) {
    free(msg);
}
