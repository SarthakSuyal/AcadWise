
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "AcadWise.h"

#define DATA_FILE "tasks.txt"
#define TEMP_FILE "tasks.tmp"
#define BACKUP_FILE "tasks.bak"
#define OLD_FILE "tasks.dat"
#define ID_FILE "next_id.dat"
#define LINE_SIZE 512

// ========================================
// TEXT ENCODING
// ========================================

static void encodeText(const char *src, char *dest) {
    const char *hex = "0123456789ABCDEF";

    while (*src) {
        unsigned char c = (unsigned char)*src++;
        *dest++ = hex[c >> 4];
        *dest++ = hex[c & 15];
    }

    *dest = '\0';
}

static int hexValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static int decodeText(const char *src,
                      char *dest, size_t capacity) {
    size_t len = strlen(src);

    if (len % 2 != 0 || len / 2 >= capacity)
        return 0;

    for (size_t i = 0; i < len; i += 2) {
        int a = hexValue(src[i]);
        int b = hexValue(src[i + 1]);

        if (a < 0 || b < 0)
            return 0;

        dest[i / 2] = (char)((a << 4) | b);

        if (dest[i / 2] == '\0')
            return 0;
    }

    dest[len / 2] = '\0';
    return 1;
}

// ========================================
// FILE UTILITIES
// ========================================

// Check whether a file exists
static int fileExists(const char *path) {
    FILE *file = fopen(path, "rb");

    if (file == NULL)
        return 0;

    fclose(file);
    return 1;
}

// Copy a file without modifying the source
static int copyFile(const char *source,
                    const char *destination) {
    FILE *in = fopen(source, "rb");

    if (in == NULL)
        return 0;

    FILE *out = fopen(destination, "wb");

    if (out == NULL) {
        fclose(in);
        return 0;
    }

    unsigned char buffer[4096];
    size_t count;
    int success = 1;

    while ((count = fread(buffer, 1,
                          sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, count, out) != count) {
            success = 0;
            break;
        }
    }

    if (ferror(in))
        success = 0;

    if (fclose(out) != 0)
        success = 0;

    if (fclose(in) != 0)
        success = 0;

    if (!success)
        remove(destination);

    return success;
}

// ========================================
// WRITE PORTABLE TASK DATA
// ========================================

static int writeTaskData(FILE *file) {
    if (fprintf(file, "ACADWISE_V1\n%d %d\n",
                taskCount, nextTaskID) < 0)
        return 0;

    for (int i = 0; i < taskCount; i++) {
        Task *t = &tasks[i];

        char titleHex[sizeof(t->title) * 2 + 1];
        char subjectHex[sizeof(t->subject) * 2 + 1];

        encodeText(t->title, titleHex);
        encodeText(t->subject, subjectHex);

        if (fprintf(file,
            "%d %d %d %d %d %d %.9g %.9g %d\n",
            t->id,
            t->dueDay,
            t->dueMonth,
            t->dueYear,
            t->importance,
            t->difficulty,
            (double)t->estimatedHours,
            (double)t->remainingHours,
            t->completed) < 0)
            return 0;

        if (fprintf(file, "%s\n%s\n",
                    titleHex, subjectHex) < 0)
            return 0;
    }

    return 1;
}

// ========================================
// SAFE SAVE WITH BACKUP
// ========================================

void saveTasks(void) {
    FILE *file = fopen(TEMP_FILE, "wb");

    if (file == NULL) {
        printf("\nError: Cannot create temporary file.\n");
        return;
    }

    int success = writeTaskData(file);

    if (fflush(file) != 0)
        success = 0;

    if (fclose(file) != 0)
        success = 0;

    if (!success) {
        remove(TEMP_FILE);
        printf("\nError: Task data could not be written.\n");
        return;
    }

    int hadPrimary = fileExists(DATA_FILE);

    // Preserve the last primary file before replacing it
    if (hadPrimary) {
        // Do not destroy an existing backup if copying fails
        if (!copyFile(DATA_FILE, "tasks.bak.tmp")) {
            remove(TEMP_FILE);
            printf("\nError: Could not prepare backup.\n");
            return;
        }

        // Replace the previous backup
        if (fileExists(BACKUP_FILE) &&
            remove(BACKUP_FILE) != 0) {
            remove("tasks.bak.tmp");
            remove(TEMP_FILE);
            printf("\nError: Could not update backup.\n");
            return;
        }

        if (rename("tasks.bak.tmp", BACKUP_FILE) != 0) {
            remove("tasks.bak.tmp");
            remove(TEMP_FILE);
            printf("\nError: Could not finalize backup.\n");
            return;
        }
    }

    /*
     * Try direct replacement first.
     * Some systems permit replacing an existing file.
     */
    if (rename(TEMP_FILE, DATA_FILE) == 0) {
        printf("\nTasks saved successfully!\n");
        return;
    }

    /*
     * Fallback for systems that do not permit
     * renaming over an existing destination.
     */
    if (hadPrimary) {
        if (remove(DATA_FILE) != 0) {
            remove(TEMP_FILE);
            printf("\nError: Could not replace task file.\n");
            return;
        }
    }

    if (rename(TEMP_FILE, DATA_FILE) == 0) {
        printf("\nTasks saved successfully!\n");
        return;
    }

    // Attempt restoration after failed replacement
    if (hadPrimary) {
        if (!copyFile(BACKUP_FILE, DATA_FILE)) {
            printf("\nCRITICAL: Automatic restore failed!\n");
            printf("Preserve tasks.bak and tasks.tmp.\n");
            return;
        }
    }

    remove(TEMP_FILE);
    printf("\nError: Save failed. Previous data restored.\n");
}

// ========================================
// LOAD PORTABLE TEXT FILE
// ========================================

static int loadTextFile(const char *path) {
    FILE *file = fopen(path, "rb");

    if (file == NULL)
        return 0;

    char line[LINE_SIZE];
    int count, savedNextID;

    if (!fgets(line, sizeof(line), file) ||
        strcmp(line, "ACADWISE_V1\n") != 0 ||
        !fgets(line, sizeof(line), file) ||
        sscanf(line, "%d %d",
               &count, &savedNextID) != 2 ||
        count < 0 || count > MAX_TASKS ||
        savedNextID < 101) {
        fclose(file);
        return -1;
    }

    Task loaded[MAX_TASKS] = {0};
    int maxID = 100;

    for (int i = 0; i < count; i++) {
        Task *t = &loaded[i];

        if (!fgets(line, sizeof(line), file) ||
            sscanf(line,
                "%d %d %d %d %d %d %f %f %d",
                &t->id,
                &t->dueDay,
                &t->dueMonth,
                &t->dueYear,
                &t->importance,
                &t->difficulty,
                &t->estimatedHours,
                &t->remainingHours,
                &t->completed) != 9) {
            fclose(file);
            return -1;
        }

        char titleHex[201];
        char subjectHex[101];

        if (!fgets(titleHex, sizeof(titleHex), file) ||
            !fgets(subjectHex, sizeof(subjectHex), file)) {
            fclose(file);
            return -1;
        }

        titleHex[strcspn(titleHex, "\r\n")] = '\0';
        subjectHex[strcspn(subjectHex, "\r\n")] = '\0';

        if (!decodeText(titleHex, t->title,
                        sizeof(t->title)) ||
            !decodeText(subjectHex, t->subject,
                        sizeof(t->subject))) {
            fclose(file);
            return -1;
        }

        if (t->id < 101 || t->id == INT_MAX ||
            t->dueMonth < 1 || t->dueMonth > 12 ||
            t->dueDay < 1 || t->dueDay > 31 ||
            t->dueYear < 1900 || t->dueYear > 9999 ||
            t->importance < 1 || t->importance > 5 ||
            t->difficulty < 1 || t->difficulty > 5 ||
            !isfinite(t->estimatedHours) ||
            !isfinite(t->remainingHours) ||
            t->estimatedHours <= 0 ||
            t->remainingHours < 0 ||
            t->remainingHours > t->estimatedHours ||
            (t->completed != 0 && t->completed != 1)) {
            fclose(file);
            return -1;
        }

        for (int j = 0; j < i; j++) {
            if (loaded[j].id == t->id) {
                fclose(file);
                return -1;
            }
        }

        t->daysRemaining = calculateDaysRemaining(
            t->dueDay, t->dueMonth, t->dueYear
        );

        if (t->daysRemaining == -999999) {
            fclose(file);
            return -1;
        }

        t->priorityScore = 0;

        if (t->id > maxID)
            maxID = t->id;
    }

    // Reject unexpected extra data
    if (fgetc(file) != EOF || ferror(file)) {
        fclose(file);
        return -1;
    }

    fclose(file);

    if (savedNextID <= maxID)
        return -1;

    for (int i = 0; i < count; i++)
        tasks[i] = loaded[i];

    taskCount = count;
    nextTaskID = savedNextID;

    return 1;
}

// ========================================
// IMPORT OLD BINARY STORAGE
// ========================================

static int importOldTasks(void) {
    FILE *file = fopen(OLD_FILE, "rb");

    if (file == NULL)
        return 0;

    int count;

    if (fread(&count, sizeof(count), 1, file) != 1 ||
        count < 0 || count > MAX_TASKS) {
        fclose(file);
        return -1;
    }

    Task loaded[MAX_TASKS] = {0};

    if (count > 0 &&
        fread(loaded, sizeof(Task),
              (size_t)count, file) != (size_t)count) {
        fclose(file);
        return -1;
    }

    fclose(file);

    int maxID = 100;

    for (int i = 0; i < count; i++) {
        Task *t = &loaded[i];

        if (t->id < 101 || t->id == INT_MAX ||
            memchr(t->title, '\0',
                   sizeof(t->title)) == NULL ||
            memchr(t->subject, '\0',
                   sizeof(t->subject)) == NULL ||
            calculateDaysRemaining(
                t->dueDay, t->dueMonth,
                t->dueYear) == -999999) {
            return -1;
        }

        if (t->id > maxID)
            maxID = t->id;
    }

    int recoveredID = maxID + 1;

    FILE *idFile = fopen(ID_FILE, "r");

    if (idFile != NULL) {
        int storedID;

        if (fscanf(idFile, "%d", &storedID) == 1 &&
            storedID > recoveredID) {
            recoveredID = storedID;
        }

        fclose(idFile);
    }

    for (int i = 0; i < count; i++)
        tasks[i] = loaded[i];

    taskCount = count;
    nextTaskID = recoveredID;

    updateDeadlines();
    return 1;
}

// ========================================
// MAIN LOADING FUNCTION
// ========================================

void loadTasks(void) {
    int result = loadTextFile(DATA_FILE);

    if (result == 1) {
        printf("\n%d task(s) restored!\n", taskCount);
        return;
    }

    if (result == -1) {
        printf("\nPrimary task file is invalid.\n");
        printf("Attempting backup recovery...\n");

        result = loadTextFile(BACKUP_FILE);

        if (result == 1) {
            printf("\nBackup recovered successfully!\n");
            printf("Please save to restore the primary file.\n");
            return;
        }

        printf("\nERROR: No valid backup available.\n");
        printf("Data files have not been overwritten.\n");
        exit(EXIT_FAILURE);
    }

    // Primary file is missing
    result = loadTextFile(BACKUP_FILE);

    if (result == 1) {
        printf("\nRecovered tasks from backup!\n");
        return;
    }

    if (result == -1) {
        printf("\nInvalid backup file. Startup stopped.\n");
        exit(EXIT_FAILURE);
    }

    // Try old binary storage if no text files exist
    result = importOldTasks();

    if (result == 1) {
        printf("\nOld task data imported!\n");
        saveTasks();
        return;
    }

    if (result == -1) {
        printf("\nOld binary data could not be imported.\n");
        printf("Preserve the original file.\n");
        exit(EXIT_FAILURE);
    }

    taskCount = 0;
    nextTaskID = 101;

    printf("\nNo saved tasks. Starting fresh.\n");
}
