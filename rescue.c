
#include <stdio.h>
#include "AcadWise.h"

#define MAX_RECOVERY_DAYS 30
#define EPSILON 0.0001f

// ========================================
// ACADEMIC RESCUE MODE
// ========================================

void academicRescueMode(void) {
    int indices[MAX_TASKS];
    float remaining[MAX_TASKS];

    int count = 0;
    int overdue = 0;
    int horizonTasks = 0;
    int atRisk = 0;
    int planningDays = 0;

    float dailyHours;
    float totalScheduled = 0.0f;
    float totalUnscheduled = 0.0f;
    float totalDeferred = 0.0f;

    updateDeadlines();
    printHeader("ACADEMIC RESCUE MODE");

    // Collect pending assignments
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed &&
            tasks[i].remainingHours > EPSILON) {

            indices[count] = i;
            remaining[count] = tasks[i].remainingHours;
            count++;
        }
    }

    if (count == 0) {
        printSuccess("No pending tasks to recover!");
        return;
    }

    dailyHours = readFloat(
        "\nAvailable study hours per day (0.5-24): ",
        0.5f, 24.0f
    );

    int startChoice = readInt(
        "Start recovery (0=Today, 1=Tomorrow): ",
        0, 1
    );

    // Sort by earliest deadline
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int first = indices[j];
            int second = indices[j + 1];

            if (tasks[first].daysRemaining >
                tasks[second].daysRemaining) {

                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;

                float tempHours = remaining[j];
                remaining[j] = remaining[j + 1];
                remaining[j + 1] = tempHours;
            }
        }
    }

    // Determine planning horizon
    for (int i = 0; i < count; i++) {
        Task *t = &tasks[indices[i]];

        if (t->daysRemaining < 0) {
            overdue++;
            continue;
        }

        if (t->daysRemaining >= MAX_RECOVERY_DAYS) {
            horizonTasks++;
            continue;
        }

        int days = t->daysRemaining + 1;

        if (days > planningDays)
            planningDays = days;
    }

    printHeader("REVISED STUDY SCHEDULE");

    printf("Daily Capacity : %.1f hours\n", dailyHours);
    printf("Recovery Starts: %s\n",
           startChoice == 0 ? "Today" : "Tomorrow");

    // Generate day-by-day schedule
    for (int day = startChoice;
         day < planningDays;
         day++) {

        float available = dailyHours;
        float used = 0.0f;

        printf("\n");
        printSeparator();

        if (day == 0)
            printf("DAY 1 - TODAY\n");
        else if (day == 1)
            printf("DAY 2 - TOMORROW\n");
        else
            printf("DAY %d\n", day + 1);

        printSeparator();

        printf("%-6s %-18s %-15s %-7s\n",
               "ID", "TASK", "SUBJECT", "HOURS");

        int scheduledToday = 0;

        for (int i = 0; i < count; i++) {
            Task *t = &tasks[indices[i]];

            // Skip overdue and distant tasks
            if (t->daysRemaining < day ||
                t->daysRemaining >= MAX_RECOVERY_DAYS)
                continue;

            if (remaining[i] <= EPSILON ||
                available <= EPSILON)
                continue;

            float allocated = remaining[i];

            if (allocated > available)
                allocated = available;

            printf("%-6d %-18.18s %-15.15s %6.1f\n",
                   t->id,
                   t->title,
                   t->subject,
                   allocated);

            remaining[i] -= allocated;
            available -= allocated;
            used += allocated;
            totalScheduled += allocated;
            scheduledToday++;
        }

        if (scheduledToday == 0)
            printf("No work scheduled.\n");

        printf("\nDaily Total: %.1f / %.1f hours\n",
               used, dailyHours);
    }

    // ========================================
    // RECOVERY REPORT
    // ========================================

    printHeader("RECOVERY SUMMARY");

    printf("Total Scheduled : %.1f hours\n",
           totalScheduled);

    for (int i = 0; i < count; i++) {
        Task *t = &tasks[indices[i]];

        if (remaining[i] <= EPSILON)
            continue;

        if (t->daysRemaining >= MAX_RECOVERY_DAYS) {
            totalDeferred += remaining[i];
            continue;
        }

        totalUnscheduled += remaining[i];
        atRisk++;

        printf("\n[!] Task ID: %d\n", t->id);
        printf("Task         : %s\n", t->title);
        printf("Subject      : %s\n", t->subject);
        printf("Unscheduled  : %.1f hours\n",
               remaining[i]);

        if (t->daysRemaining < 0) {
            printf("Reason       : Already overdue\n");
        } else if (t->daysRemaining < startChoice) {
            printf("Reason       : Recovery starts too late\n");
        } else {
            printf("Reason       : Insufficient study time\n");
        }
    }

    printSeparator();

    printf("Overdue Tasks       : %d\n", overdue);
    printf("Tasks At Risk       : %d\n", atRisk);
    printf("Unscheduled Work    : %.1f hours\n",
           totalUnscheduled);

    if (atRisk == 0) {
        printSuccess(
            "All tasks within the planning horizon fit!"
        );
    } else {
        printError("Recovery plan has deadline conflicts.");

        printf("\nSuggestions:\n");
        printf("1. Increase available study hours.\n");
        printf("2. Start recovery earlier if possible.\n");
        printf("3. Focus on urgent assignments.\n");
    }

    if (horizonTasks > 0) {
        printf("\nTasks Beyond 30-Day Horizon: %d\n",
               horizonTasks);
        printf("Work Deferred: %.1f hours\n",
               totalDeferred);
        printf("These tasks were not evaluated for\n");
        printf("deadline feasibility in this plan.\n");
    }

    printf("\nNOTE: This is a suggested schedule.\n");
    printf("Actual task progress remains unchanged.\n");

    printSeparator();
}
