
#include <stdio.h>
#include "AcadWise.h"

void generateStudyPlan(void) {
    int indices[MAX_TASKS];
    int count = 0;

    float availableHours;
    float remainingTime;
    float totalAllocated = 0.0f;

    // Refresh deadlines
    updateDeadlines();

    printHeader("DAILY STUDY PLANNER");

    // Collect pending tasks
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed &&
            tasks[i].remainingHours > 0.0f) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printError("No pending tasks to schedule!");
        return;
    }

    availableHours = readFloat(
        "\nAvailable study hours today (0.5-24): ",
        0.5f, 24.0f
    );

    remainingTime = availableHours;

    // Sort by priority (highest first)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int first = indices[j];
            int second = indices[j + 1];

            if (calculatePriority(tasks[first]) <
                calculatePriority(tasks[second])) {

                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    printf("\n%-6s %-18s %-15s %-8s %-8s\n",
           "ID", "TASK", "SUBJECT", "DAYS", "TIME");

    printf("-------------------------------------------------------------\n");

    int scheduledCount = 0;

    for (int i = 0; i < count; i++) {
        if (remainingTime <= 0.0001f)
            break;

        Task *task = &tasks[indices[i]];

        float allocatedHours = task->remainingHours;

        // Allow partial assignment work
        if (allocatedHours > remainingTime)
            allocatedHours = remainingTime;

        printf("%-6d %-18.18s %-15.15s %-8d %6.1fh\n",
               task->id,
               task->title,
               task->subject,
               task->daysRemaining,
               allocatedHours);

        remainingTime -= allocatedHours;
        totalAllocated += allocatedHours;
        scheduledCount++;
    }

    if (remainingTime < 0.0001f)
        remainingTime = 0.0f;

    printf("-------------------------------------------------------------\n");

    printf("\nTasks Scheduled : %d\n", scheduledCount);
    printf("Available Time  : %.1f hours\n", availableHours);
    printf("Total Allocated : %.1f hours\n", totalAllocated);
    printf("Remaining Time  : %.1f hours\n", remainingTime);

    printSeparator();

    printSuccess("Study plan generated successfully!");

    printf("\nNOTE: This is a suggested plan only.\n");
    printf("Task progress has not been changed.\n");
}
