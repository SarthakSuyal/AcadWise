
#include <stdio.h>
#include <string.h>
#include "AcadWise.h"

void showDashboard(void) {
    int completed = 0;
    int pending = 0;
    int urgent = 0;
    int overdue = 0;

    float pendingHours = 0.0f;
    float completionRate = 0.0f;

    // Store unique subjects and their workload
    char subjects[MAX_TASKS][50] = {{0}};
    float subjectHours[MAX_TASKS] = {0.0f};
    int subjectCount = 0;

    // Refresh deadline countdowns
    updateDeadlines();

    printf("\n========================================\n");
    printf("       ACADWISE PROGRESS DASHBOARD\n");
    printf("========================================\n");

    if (taskCount == 0) {
        printf("\nNo tasks available yet.\n");
        printf("Add your first academic task!\n");
        return;
    }

    // Analyze all tasks
    for (int i = 0; i < taskCount; i++) {
        Task *t = &tasks[i];

        if (t->completed) {
            completed++;
            continue;
        }

        pending++;
        pendingHours += t->remainingHours;

        // Overdue tasks
        if (t->daysRemaining < 0) {
            overdue++;
        }
        // Urgent tasks: due today, tomorrow or in 2 days
        else if (t->daysRemaining <= 2) {
            urgent++;
        }

        // Find existing subject
        int subjectIndex = -1;

        for (int j = 0; j < subjectCount; j++) {
            if (strcmp(subjects[j], t->subject) == 0) {
                subjectIndex = j;
                break;
            }
        }

        // Add new subject if not found
        if (subjectIndex == -1) {
            subjectIndex = subjectCount;

            strcpy(subjects[subjectCount], t->subject);
            subjectHours[subjectCount] = 0.0f;

            subjectCount++;
        }

        subjectHours[subjectIndex] += t->remainingHours;
    }

    // Calculate completion percentage
    completionRate =
        ((float)completed / taskCount) * 100.0f;

    printf("\nTotal Tasks       : %d\n", taskCount);
    printf("Completed Tasks   : %d\n", completed);
    printf("Pending Tasks     : %d\n", pending);

    printf("\nCompletion Rate   : %.1f%%\n",
           completionRate);

    // ASCII progress bar
    printProgressBar(completionRate);

    printf("] %.1f%%\n", completionRate);

    printf("\n----------------------------------------\n");
    printf("           DEADLINE OVERVIEW\n");
    printf("----------------------------------------\n");

    printf("Urgent Tasks      : %d\n", urgent);
    printf("Overdue Tasks     : %d\n", overdue);
    printf("Pending Workload  : %.1f Hours\n",
           pendingHours);

    printf("\n----------------------------------------\n");
    printf("         SUBJECT-WISE WORKLOAD\n");
    printf("----------------------------------------\n");

    if (subjectCount == 0) {
        printf("\nNo pending workload!\n");
    } else {
        for (int i = 0; i < subjectCount; i++) {
            printf("%-20.20s : %.1f Hours\n",
                   subjects[i],
                   subjectHours[i]);
        }
    }

    printf("\n========================================\n");
}
