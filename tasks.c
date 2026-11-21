
#include <stdio.h>
#include "AcadWise.h"
#include <limits.h>

// Shared task storage
Task tasks[MAX_TASKS];
int taskCount = 0;
int nextTaskID = 101;


// ========================================
// ADD NEW TASK
// ========================================

void addTask(void) {
    if (taskCount >= MAX_TASKS) {
        printf("\nTask storage is full!\n");
        return;
    }

    Task *t = &tasks[taskCount];

    // Generate a unique ID among existing tasks
    if (nextTaskID == INT_MAX) {
        printf("\nTask ID limit reached!\n");
        return;
    }

    t->id = nextTaskID++;

    printf("\n========== ADD NEW TASK ==========\n");

    readString("Task Title: ",
               t->title, sizeof(t->title));

    readString("Subject: ",
               t->subject, sizeof(t->subject));

    // Get and validate assignment deadline
    printf("\n----- Assignment Deadline -----\n");

    do {
        t->dueDay =
            readInt("Day (1-31): ", 1, 31);

        t->dueMonth =
            readInt("Month (1-12): ", 1, 12);

        t->dueYear =
            readInt("Year (2026-2100): ", 2026, 2100);

        t->daysRemaining = calculateDaysRemaining(
            t->dueDay,
            t->dueMonth,
            t->dueYear
        );

        if (t->daysRemaining == -999999) {
            printf("\nInvalid calendar date! "
                   "Please enter a valid date.\n\n");
        }

    } while (t->daysRemaining == -999999);

    t->importance =
        readInt("\nImportance (1-5): ", 1, 5);

    t->difficulty =
        readInt("Difficulty (1-5): ", 1, 5);

    t->estimatedHours =
        readFloat("Estimated hours (0.5-100): ",
                  0.5f, 100.0f);

    t->remainingHours = t->estimatedHours;
    t->completed = 0;
    t->priorityScore = 0;

    taskCount++;
    saveTasks();

    printf("\n====================================\n");
    printf("Task added successfully!\n");
    printf("Task ID: %d\n", t->id);

    printf("Deadline: %02d/%02d/%04d\n",
           t->dueDay,
           t->dueMonth,
           t->dueYear);

    if (t->daysRemaining < 0) {
        printf("Warning: Deadline already passed "
               "(%d day(s) overdue).\n",
               -t->daysRemaining);
    } else if (t->daysRemaining == 0) {
        printf("Deadline Status: DUE TODAY!\n");
    } else {
        printf("Days Remaining: %d\n",
               t->daysRemaining);
    }

    printf("====================================\n");
}


// ========================================
// VIEW ALL TASKS
// ========================================


void viewTasks(void) {
    updateDeadlines();

    printHeader("ALL ACADEMIC TASKS");

    if (taskCount == 0) {
        printError("No tasks available.");
        return;
    }

    int completed = 0;
    int pending = 0;

    for (int i = 0; i < taskCount; i++) {
        Task *t = &tasks[i];

        printf("\nTask ID   : %d\n", t->id);
        printf("Title     : %s\n", t->title);
        printf("Subject   : %s\n", t->subject);

        printf("Deadline  : %02d/%02d/%04d\n",
               t->dueDay, t->dueMonth, t->dueYear);

        if (t->completed) {
            printf("Deadline  : Completed\n");
            completed++;
        } else {
            pending++;

            if (t->daysRemaining < 0) {
                printf("Countdown : OVERDUE by %d day(s)\n",
                       -t->daysRemaining);
            } else if (t->daysRemaining == 0) {
                printf("Countdown : DUE TODAY!\n");
            } else {
                printf("Countdown : %d day(s) remaining\n",
                       t->daysRemaining);
            }
        }

        printf("Workload  : %.1f / %.1f hours remaining\n",
               t->remainingHours, t->estimatedHours);

        printf("Importance: %d/5\n", t->importance);
        printf("Difficulty: %d/5\n", t->difficulty);

        printf("Status    : %s\n",
               t->completed ? "COMPLETED" : "PENDING");

        printSeparator();
    }

    printf("\nTotal Tasks     : %d\n", taskCount);
    printf("Pending Tasks   : %d\n", pending);
    printf("Completed Tasks : %d\n", completed);
}

// ========================================
// MARK TASK AS COMPLETED
// ========================================

void completeTask(void) {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    int id = readInt(
        "\nEnter Task ID to complete: ",
        101, 2147483647
    );

    for (int i = 0; i < taskCount; i++) {

        if (tasks[i].id == id) {

            if (tasks[i].completed) {
                printf("\nTask is already completed.\n");
                return;
            }

            tasks[i].completed = 1;
            tasks[i].remainingHours = 0.0f;
            saveTasks();

            printf("\nTask completed successfully!\n");
            printf("Task: %s\n", tasks[i].title);
            printf("Subject: %s\n", tasks[i].subject);

            return;
        }
    }

    printf("\nTask ID not found.\n");
}


// ========================================
// DELETE TASK
// ========================================

void deleteTask(void) {
    if (taskCount == 0) {
        printf("\nNo tasks available to delete!\n");
        return;
    }

    int id = readInt(
        "\nEnter Task ID to delete: ",
        101, 2147483647
    );

    int index = -1;

    // Find the task
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nTask ID not found!\n");
        return;
    }

    // Display task information
    printf("\n========== DELETE TASK ==========\n");

    printf("Task ID: %d\n", tasks[index].id);
    printf("Title: %s\n", tasks[index].title);
    printf("Subject: %s\n", tasks[index].subject);

    printf("Deadline: %02d/%02d/%04d\n",
           tasks[index].dueDay,
           tasks[index].dueMonth,
           tasks[index].dueYear);

    printf("Status: %s\n",
           tasks[index].completed ?
           "Completed" : "Pending");

    // Confirmation
    int confirm = readInt(
        "\nAre you sure you want to delete? "
        "(1=Yes, 0=No): ",
        0, 1
    );

    if (confirm == 0) {
        printf("\nDeletion cancelled.\n");
        return;
    }

    // Shift subsequent tasks left
    for (int i = index; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    taskCount--;

    // Save changes to storage
    saveTasks();

    printf("\nTask deleted successfully!\n");
    printf("Remaining Tasks: %d\n", taskCount);
}


void updateTaskProgress(void) {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    printf("\n====================================\n");
    printf("         UPDATE TASK PROGRESS\n");
    printf("====================================\n");

    // Display pending tasks
    int pendingCount = 0;

    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed) {
            printf("\nID: %d | %s\n",
                   tasks[i].id,
                   tasks[i].title);

            printf("Subject: %s\n",
                   tasks[i].subject);

            printf("Remaining Work: %.1f hours\n",
                   tasks[i].remainingHours);

            pendingCount++;
        }
    }

    if (pendingCount == 0) {
        printf("\nAll tasks are completed!\n");
        return;
    }

    int id = readInt(
        "\nEnter Task ID to update: ",
        101, 2147483647
    );

    int index = -1;

    // Find the selected task
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nTask ID not found!\n");
        return;
    }

    Task *t = &tasks[index];

    if (t->completed) {
        printf("\nThis task is already completed.\n");
        return;
    }

    printf("\nSelected Task: %s\n", t->title);
    printf("Estimated Hours: %.1f\n",
           t->estimatedHours);
    printf("Remaining Hours: %.1f\n",
           t->remainingHours);

    // Record actual study hours
    float studiedHours = readFloat(
        "\nHow many hours did you work? ",
        0.1f, 24.0f
    );

    if (studiedHours > t->remainingHours) {
        printf("\nEntered hours exceed remaining work.\n");
        printf("Maximum allowed: %.1f hours\n",
               t->remainingHours);
        return;
    }

    t->remainingHours -= studiedHours;

    // Handle small floating-point differences
    if (t->remainingHours < 0.001f) {
        t->remainingHours = 0.0f;
        t->completed = 1;
    }

    // Calculate completion percentage
    float progress =
        ((t->estimatedHours - t->remainingHours)
         / t->estimatedHours) * 100.0f;

    printf("\n====================================\n");
    printf("          PROGRESS UPDATED\n");
    printf("====================================\n");

    printf("Task: %s\n", t->title);
    printf("Hours Studied: %.1f\n", studiedHours);
    printf("Remaining Work: %.1f hours\n",
           t->remainingHours);

    printf("Completion: %.1f%%\n", progress);

    // ASCII progress bar
    int filled = (int)(progress / 5.0f);

    printf("\nProgress: [");

    for (int i = 0; i < 20; i++) {
        if (i < filled)
            printf("#");
        else
            printf("-");
    }

    printf("] %.1f%%\n", progress);

    if (t->completed) {
        printf("\nTask completed successfully!\n");
    } else {
        printf("\nKeep going! You're making progress.\n");
    }

    // Save progress immediately
    saveTasks();
}
