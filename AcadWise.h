#ifndef ACADWISE_H
#define ACADWISE_H

#define MAX_TASKS 100

typedef struct {
    int id;
    char title[100];
    char subject[50];

    int dueDay;
    int dueMonth;
    int dueYear;
    int daysRemaining;
    int importance;
    int difficulty;

    float estimatedHours;
    float remainingHours;

    int completed;
    int priorityScore;
} Task;

// Shared task records
extern Task tasks[MAX_TASKS];
extern int taskCount;
extern int nextTaskID;

// Task management
int calculateDaysRemaining(int day, int month, int year);
void updateDeadlines(void);
void addTask(void);
void viewTasks(void);
void completeTask(void);
void deleteTask(void);
void updateTaskProgress(void);

// Priority engine
int calculatePriority(Task task);
void showPriorityRanking(void);

// Study planner
void generateStudyPlan(void);

// Deadline analysis
void analyzeDeadlineRisk(void);

// Recovery system
void academicRescueMode(void);

// Dashboard
void showDashboard(void);

// Input validation
int readInt(const char *prompt, int min, int max);
float readFloat(const char *prompt, float min, float max);
void readString(const char *prompt, char *buffer, int size);

//Data Storage
void saveTasks(void);
void loadTasks(void);

// Terminal UI
void printSeparator(void);
void printHeader(const char *title);
void printSuccess(const char *message);
void printError(const char *message);
void printProgressBar(float percentage);
void pauseScreen(void);

#endif
