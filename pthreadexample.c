#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

// Global variables
pthread_mutex_t mutex;
bool exitFlag = false;
int i = 0;
char userInput[32]; // Array to store user input

// Function to be executed by the thread
void *printHelloWorld(void *arg) {
    while (!exitFlag) {
        // Lock the mutex to ensure exclusive access to the console
        pthread_mutex_lock(&mutex);

        // Clear the screen
        system("clear");

        // Print "Hello, World!" I times
        for (int j = 0; j <= i; j++) {
        	printf("Hello, World! (%d)\n", j + 1);
        }

        // Print user input
        printf("User Input: %s\n", userInput);

        // Unlock the mutex to release access to the console
        pthread_mutex_unlock(&mutex);

        // Increment the variable i
        i++;

        // Sleep for a short duration to avoid busy-waiting
        usleep(1000000);  // 500 milliseconds
    }

    return NULL;
}

int main() {
    pthread_t thread;

    // Initialize the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Mutex initialization failed\n");
        return EXIT_FAILURE;
    }

    // Create the thread
    if (pthread_create(&thread, NULL, printHelloWorld, NULL) != 0) {
        fprintf(stderr, "Thread creation failed\n");
        return EXIT_FAILURE;
    }

    // Main thread: Wait for user input
    // Loop to capture user input
    char c;
    while ((c = getchar()) != '\n') {
        // Lock the mutex to ensure exclusive access to the user input array
        pthread_mutex_lock(&mutex);

        // Append the pressed key to the user input array
        strncat(userInput, &c, 1);

        // Unlock the mutex to release access to the user input array
        pthread_mutex_unlock(&mutex);
    }

    // Set the exit flag to signal the thread to terminate
    exitFlag = true;

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    // Clean up: Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}
