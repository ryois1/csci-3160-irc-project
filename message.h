#ifndef MESSAGE_PROTOCOL_H
#define MESSAGE_PROTOCOL_H

#include <stdint.h>

// Define message types
typedef enum {
    MSG_TYPE_JOIN = 1,
    MSG_TYPE_DATA = 2,
    MSG_TYPE_DELETE = 3,
    // Add more message types as needed
} MessageType;

// Define the maximum size of the payload
#define MAX_PAYLOAD_SIZE 256

// Define the message structure
typedef struct {
    MessageType type;
    uint16_t length;  // Length of the payload
    uint8_t payload[MAX_PAYLOAD_SIZE];
} Message;

// Function to create a new message
uint8_t* encodeMessage(Message message){
    //TODO return payload prefaced with datatype.
    return message.payload;
}

// Function to extract data from a message
void extractData(uint8_t* buffer){
        // Check if the message type is suitable for data extraction
        //Check buffer byte [0] == Data
        // Handle error - this message type doesn't contain data
        //TODO return message object created from bits.
}

#endif // MESSAGE_PROTOCOL_H
