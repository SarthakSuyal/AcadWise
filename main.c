
#include <stdio.h>
#include "AcadWise.h"


void showMenu(void) {
    printHeader("ACADWISE v1.0");

    printf("  SMART ACADEMIC PRIORITY & RECOVERY\n");
    printf("  Plan Smarter. Meet Deadlines.\n\n");

    printSeparator();

    printf("  TASK MANAGEMENT\n\n");
    printf("  [1]  Add New Task\n");
    printf("  [2]  View All Tasks\n");
    printf("  [7]  Mark Task Completed\n");
    printf("  [9]  Delete Task\n");
    printf("  [10] Update Task Progress\n\n");

    printSeparator();

    printf("  SMART PLANNING\n\n");
    printf("  [3]  Smart Priority Ranking\n");
    printf("  [4]  Daily Study Scheduler\n");
    printf("  [5]  Deadline Risk Analyzer\n");
    printf("  [6]  Academic Rescue Mode\n\n");

    printSeparator();

    printf("  ANALYTICS\n\n");
    printf("  [8]  Academic Progress Dashboard\n\n");

    printSeparator();

    printf("  [0]  Save & Exit\n");

    printSeparator();

    printf("\n  Enter your choice: ");
}


int main(void) {
    int choice;
    loadTasks();
    updateDeadlines();
    do {
        updateDeadlines();
        showMenu();

        choice = readInt("", 0, 10);

        switch (choice) {
            case 1:
                addTask();
                break;

            case 2:
                viewTasks();
                break;

            case 3:
                showPriorityRanking();
                break;

            case 4:
                generateStudyPlan();
                break;

            case 5:
                analyzeDeadlineRisk();
                break;

            case 6:
                academicRescueMode();
                break;

            case 7:
                completeTask();
                break;

            case 8:
                showDashboard();
                break;

            case 9:
                deleteTask();
                break;
            
            case 0:
                saveTasks();
                printf("\nThank you for using AcadWise!\n");
                break;

            case 10:
                updateTaskProgress();
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
