
#include <stdio.h>
#include "AcadWise.h"

// ========================================
// CALCULATE PRIORITY SCORE
// ========================================

int calculatePriority(Task task) {
    int urgency;

    if (task.daysRemaining <= 0)
        urgency = 10;
    else if (task.daysRemaining == 1)
        urgency = 9;
    else if (task.daysRemaining <= 3)
        urgency = 7;
    else if (task.daysRemaining <= 7)
        urgency = 4;
    else
        urgency = 1;

    // Use whole hours, capped at 8
    int hours = (int)task.remainingHours;

    if (hours > 8)
        hours = 8;

    return (4 * urgency)
         + (3 * task.importance)
         + (2 * task.difficulty)
         + hours;
}


// ========================================
// DISPLAY SMART PRIORITY RANKING
// ========================================

void showPriorityRanking(void) {

    // Refresh deadline countdowns
    updateDeadlines();

    int indices[MAX_TASKS];
    int count = 0;

    // Collect pending tasks
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed &&
            tasks[i].remainingHours > 0.0f) {

            indices[count++] = i;
        }
    }

    printHeader("SMART PRIORITY RANKING");

    if (count == 0) {
        printError("No pending tasks found!");
        return;
    }

    // Calculate priority scores
    for (int i = 0; i < count; i++) {
        int index = indices[i];

        tasks[index].priorityScore =
            calculatePriority(tasks[index]);
    }

    // Sort indices using bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            int first = indices[j];
            int second = indices[j + 1];

            if (tasks[first].priorityScore <
                tasks[second].priorityScore) {

                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    // ========================================
    // DISPLAY PRIORITY TABLE
    // ========================================

    printf("\n%-5s %-6s %-20s %-18s %-7s\n",
           "RANK", "ID", "TASK", "SUBJECT", "SCORE");

    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {

        Task t = tasks[indices[i]];

        printf("%-5d %-6d %-20.20s %-18.18s %-7d\n",
               i + 1,
               t.id,
               t.title,
               t.subject,
               t.priorityScore);
    }

    printf("-------------------------------------------------------------\n");

    // ========================================
    // RECOMMENDED FIRST TASK
    // ========================================

    Task top = tasks[indices[0]];

    printf("\n");
    printHeader("RECOMMENDED FIRST TASK");

    printf("Task          : %s\n", top.title);
    printf("Subject       : %s\n", top.subject);
    printf("Priority Score: %d\n", top.priorityScore);

    // Show deadline status
    if (top.daysRemaining < 0) {
        printf("Deadline      : OVERDUE by %d day(s)\n",
               -top.daysRemaining);
    }
    else if (top.daysRemaining == 0) {
        printf("Deadline      : DUE TODAY!\n");
    }
    else {
        printf("Deadline      : %d day(s) remaining\n",
               top.daysRemaining);
    }

    printf("Importance    : %d/5\n", top.importance);
    printf("Difficulty    : %d/5\n", top.difficulty);

    printf("Remaining Work: %.1f hours\n",
           top.remainingHours);

    printSeparator();
}
