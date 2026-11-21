
#include <stdio.h>
#include "AcadWise.h"

#define EPSILON 0.0001f

// ========================================
// DEADLINE RISK ANALYZER
// ========================================

void analyzeDeadlineRisk(void) {
    int indices[MAX_TASKS];
    int count = 0;

    int overdue = 0;
    int atRisk = 0;

    float dailyHours;
    float cumulativeWork = 0.0f;

    // Refresh all deadlines
    updateDeadlines();

    printHeader("DEADLINE RISK ANALYZER");

    // Collect pending tasks
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed &&
            tasks[i].remainingHours > EPSILON) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printSuccess("No pending assignments!");
        return;
    }

    dailyHours = readFloat(
        "\nAvailable study hours per day (0.5-24): ",
        0.5f, 24.0f
    );

    // Sort tasks by earliest deadline
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int first = indices[j];
            int second = indices[j + 1];

            if (tasks[first].daysRemaining >
                tasks[second].daysRemaining) {

                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    // ========================================
    // TASK OVERVIEW
    // ========================================

    printf("\n%-6s %-18s %-15s %-7s %-8s\n",
           "ID", "TASK", "SUBJECT", "DAYS", "WORK");

    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        Task *t = &tasks[indices[i]];

        printf("%-6d %-18.18s %-15.15s %-7d %6.1fh\n",
               t->id,
               t->title,
               t->subject,
               t->daysRemaining,
               t->remainingHours);
    }

    printf("-------------------------------------------------------------\n");

    // ========================================
    // DEADLINE FEASIBILITY ANALYSIS
    // ========================================

    printHeader("DEADLINE ANALYSIS");

    int i = 0;

    while (i < count) {
        int deadline = tasks[indices[i]].daysRemaining;

        // Process overdue tasks separately
        if (deadline < 0) {
            while (i < count &&
                   tasks[indices[i]].daysRemaining < 0) {

                Task *t = &tasks[indices[i]];

                printf("\n[OVERDUE] %s (ID: %d)\n",
                       t->title, t->id);

                printf("Overdue by: %d day(s)\n",
                       -t->daysRemaining);

                printf("Remaining Work: %.1f hours\n",
                       t->remainingHours);

                overdue++;
                i++;
            }

            continue;
        }

        /*
         * Combine all tasks sharing the
         * same deadline.
         */
        int groupStart = i;

        while (i < count &&
               tasks[indices[i]].daysRemaining == deadline) {

            cumulativeWork +=
                tasks[indices[i]].remainingHours;

            i++;
        }

        // Include today and deadline day
        int availableDays = deadline + 1;

        float availableCapacity =
            availableDays * dailyHours;

        float shortfall =
            cumulativeWork - availableCapacity;

        if (shortfall < 0.0f)
            shortfall = 0.0f;

        printf("\n");

        printSeparator();

        if (deadline == 0)
            printf("DEADLINE: TODAY\n");
        else
            printf("DEADLINE: IN %d DAY(S)\n", deadline);

        printf("Tasks at this deadline:\n");

        for (int j = groupStart; j < i; j++) {
            Task *t = &tasks[indices[j]];

            printf("  [%d] %-20.20s %.1f h\n",
                   t->id,
                   t->title,
                   t->remainingHours);
        }

        printf("\nCumulative Work : %.1f hours\n",
               cumulativeWork);

        printf("Available Time  : %.1f hours\n",
               availableCapacity);

        printf("Shortfall       : %.1f hours\n",
               shortfall);

        if (shortfall > EPSILON) {
            printf("Status          : AT RISK\n");
            atRisk++;
        } else {
            printf("Status          : FEASIBLE\n");
        }
    }

    // ========================================
    // FINAL RISK REPORT
    // ========================================

    printHeader("RISK SUMMARY");

    printf("Overdue Tasks       : %d\n", overdue);
    printf("At-Risk Deadlines   : %d\n", atRisk);

    if (overdue == 0 && atRisk == 0) {
        printSuccess(
            "All deadlines appear achievable!"
        );
    } else {
        printError(
            "Some deadlines need attention!"
        );

        printf("\nSuggestions:\n");
        printf("1. Increase available study hours.\n");
        printf("2. Prioritize upcoming deadlines.\n");
        printf("3. Use Academic Rescue Mode.\n");
    }

    printf("\nAssumptions:\n");
    printf("- %.1f study hours available daily.\n",
           dailyHours);
    printf("- Today and deadline day are included.\n");
    printf("- Tasks can be completed in portions.\n");

    printSeparator();
}
