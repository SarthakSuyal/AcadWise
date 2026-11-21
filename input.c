
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include "AcadWise.h"

void clearRemainingInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void readString(const char *prompt, char *buffer, int size) {
    if (size <= 1) return;

    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, size, stdin) == NULL) {
            printf("\nInput ended.\n");
            exit(EXIT_FAILURE);
        }

        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else if (!feof(stdin)) {
            clearRemainingInput();
            printf("Input too long. Try again.\n");
            continue;
        }

        if (buffer[0] == '\0') {
            printf("Input cannot be empty.\n");
            continue;
        }

        return;
    }
}

int readInt(const char *prompt, int min, int max) {
    char buffer[100];
    char *end;
    long value;

    while (1) {
        readString(prompt, buffer, sizeof(buffer));

        errno = 0;
        value = strtol(buffer, &end, 10);

        while (isspace((unsigned char)*end)) end++;

        if (errno == ERANGE || end == buffer ||
            *end != '\0' ||
            value < min || value > max) {
            printf("Enter a number between %d and %d.\n",
                   min, max);
            continue;
        }

        return (int)value;
    }
}

float readFloat(const char *prompt, float min, float max) {
    char buffer[100];
    char *end;
    float value;

    while (1) {
        readString(prompt, buffer, sizeof(buffer));

        errno = 0;
        value = strtof(buffer, &end);

        while (isspace((unsigned char)*end)) end++;

        if (errno == ERANGE || end == buffer ||
            *end != '\0' || !isfinite(value) ||
            value < min || value > max) {
            printf("Enter a value between %.1f and %.1f.\n",
                   min, max);
            continue;
        }

        return value;
    }
}
