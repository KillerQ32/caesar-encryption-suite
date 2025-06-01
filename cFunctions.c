#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cFunctions.h"
#include <time.h>

#define MAX_MESSAGES 10
#define MESSAGE_SIZE 256

// Stores up to 10 messages, each of size 256 characters
static char messages[MAX_MESSAGES][MESSAGE_SIZE] = {
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message.",
    "This is the original message."
};

// Displays all stored messages
void displayMessages() {
    for (int i = 0; i < MAX_MESSAGES; i++) {
        printf("Message[%d]: %s\n", i, messages[i]);
    }
}

// Reads a new message, validates it, and updates the current index
void readNewMessage() {
    static int currentIndex = 0;

    printf("Enter your new message: ");
    char newMessage[MESSAGE_SIZE];
    fgets(newMessage, MESSAGE_SIZE, stdin);

    // Remove trailing newline
    size_t len = strlen(newMessage);
    if (len > 0 && newMessage[len - 1] == '\n') {
        newMessage[len - 1] = '\0';
    }

    // Validate and update message
    if (strlen(newMessage) > 8 &&
        newMessage[0] >= 'A' && newMessage[0] <= 'Z' &&
        (newMessage[strlen(newMessage) - 1] == '.' ||
         newMessage[strlen(newMessage) - 1] == '?' ||
         newMessage[strlen(newMessage) - 1] == '!')) {
        strcpy(messages[currentIndex], newMessage);
        printf("Message updated.\n");
        currentIndex = (currentIndex + 1) % MAX_MESSAGES;
    } else {
        printf("Invalid message. Try again.\n");
    }
}

// Encrypts a message with a Caesar cipher using a user-defined shift
void caesarCipher() {
    printf("Enter the shift value: ");
    int shift;
    scanf("%d", &shift);

    printf("Enter the message index (0-9): ");
    int index;
    scanf("%d", &index);

    if (index < 0 || index >= MAX_MESSAGES) {
        printf("Invalid index! Try again.\n");
        return;
    }

    char output[MESSAGE_SIZE];
    encrypt_caesar(messages[index], output, shift);

    printf("Original message: %s\n", messages[index]);
    printf("Encrypted message: %s\n", output);

    strcpy(messages[index], output); // Update the message
}

// Decrypts a message by analyzing character frequencies
void frequencyDecrypt() {
    printf("Enter the message index (0-9) to decrypt: ");
    int index;
    scanf("%d", &index);

    if (index < 0 || index >= MAX_MESSAGES) {
        printf("Invalid index! Try again.\n");
        return;
    }

    char *message = messages[index];
    int frequencies[26] = {0};

    // Count character frequencies
    for (int i = 0; message[i] != '\0'; i++) {
        if (isalpha(message[i])) {
            char lower = tolower(message[i]);
            frequencies[lower - 'a']++;
        }
    }

    // Find top 5 frequent characters
    int top[5] = {-1, -1, -1, -1, -1};
    for (int i = 0; i < 5; i++) {
        int maxIndex = -1;
        for (int j = 0; j < 26; j++) {
            if ((maxIndex == -1 || frequencies[j] > frequencies[maxIndex]) &&
                (i == 0 || frequencies[j] < frequencies[top[i - 1]])) {
                maxIndex = j;
            }
        }
        top[i] = maxIndex;
    }

    // Generate possible decrypted strings
    printf("Possible decryptions for message[%d]: %s\n", index, message);
    for (int i = 0; i < 5; i++) {
        if (top[i] == -1) break;

        int shift = top[i] - ('e' - 'a');
        printf("Decryption %d (shift = %d): ", i + 1, shift);

        for (int j = 0; message[j] != '\0'; j++) {
            char c = message[j];
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                c = base + ((c - base - shift + 26) % 26);
            }
            putchar(c);
        }
        printf("\n");
    }
}

// Encrypts all messages with random shifts and updates them
void easterEgg() {
    const char *easterEggMessage = "GIVE US AN A RIGHT NOWWWWW!!!";
    srand((unsigned int)time(NULL));

    printf("\n*** EASTER EGG ACTIVATED ***\n");
    printf("Each message will be encrypted with a unique random shift!\n");

    char encrypted[MESSAGE_SIZE];
    for (int i = 0; i < MAX_MESSAGES; i++) {
        int shift = (rand() % 25) + 1;
        strcpy(messages[i], easterEggMessage);
        encrypt_caesar(messages[i], encrypted, shift);
        strcpy(messages[i], encrypted);
        printf("Message[%d] encrypted with shift %d.\n", i, shift);
    }

    printf("All messages have been updated and encrypted! Returning to the main menu...\n\n");
}
