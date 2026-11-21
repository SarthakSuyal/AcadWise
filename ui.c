
#include <stdio.h>
#include "AcadWise.h"

#define UI_WIDTH 50
#define BAR_WIDTH 20

// Print a horizontal separator
void printSeparator(void) {
    for (int i = 0; i < UI_WIDTH; i++) {
        putchar('-');
    }
    putchar('\n');
}

// Print a section heading
void printHeader(const char *title) {
    printf("\n");

    for (int i = 0; i < UI_WIDTH; i++) {
        putchar('=');
    }

    printf("\n  %s\n", title);

    for (int i = 0; i < UI_WIDTH; i++) {
        putchar('=');
    }

    printf("\n");
}

// Display success message
void printSuccess(const char *message) {
    printf("\n[+] %s\n", message);
}

// Display error message
void printError(const char *message) {
    printf("\n[!] %s\n", message);
}

// Display progress bar
void printProgressBar(float percentage) {
    if (percentage < 0.0f)
        percentage = 0.0f;

    if (percentage > 100.0f)
        percentage = 100.0f;

    int filled = (int)(
        percentage * BAR_WIDTH / 100.0f
    );

    printf("[");

    for (int i = 0; i < BAR_WIDTH; i++) {
        putchar(i < filled ? '#' : '-');
    }

    printf("] %.1f%%\n", percentage);
}

// Wait for user to continue
void pauseScreen(void) {
    char buffer[8];

    printf("\nPress Enter to continue...");
    fflush(stdout);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return;
    }

    // Discard extra characters if necessary
    if (buffer[0] != '\n') {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
}
