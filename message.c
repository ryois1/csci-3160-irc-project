// message.c
#include "message.h"
#include <stdio.h>
#include <string.h>

void format_message(char *dest, const char *username, const char *msg) {
    snprintf(dest, BUFFER_SIZE, "%s: %s", username, msg);
}
