
#include <stdio.h>
#include <time.h>
#include "AcadWise.h"

// Check leap year
static int isLeapYear(int year) {
    return (year % 400 == 0) ||
           (year % 4 == 0 && year % 100 != 0);
}

// Number of days in a month
static int daysInMonth(int month, int year) {
    const int days[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (month == 2 && isLeapYear(year))
        return 29;

    return days[month - 1];
}

// Validate a calendar date
static int validDate(int day, int month, int year) {
    if (year < 1900 || year > 9999)
        return 0;

    if (month < 1 || month > 12)
        return 0;

    return day >= 1 && day <= daysInMonth(month, year);
}

// Convert a date into a day number
static long dateToDays(int day, int month, int year) {
    long total = 0;

    for (int y = 1900; y < year; y++) {
        total += isLeapYear(y) ? 366 : 365;
    }

    for (int m = 1; m < month; m++) {
        total += daysInMonth(m, year);
    }

    return total + day;
}

// Calculate countdown using system date
int calculateDaysRemaining(int day, int month, int year) {
    if (!validDate(day, month, year))
        return -999999;

    time_t now = time(NULL);

    if (now == (time_t)-1)
        return -999999;

    struct tm *today = localtime(&now);

    if (today == NULL)
        return -999999;

    int currentDay = today->tm_mday;
    int currentMonth = today->tm_mon + 1;
    int currentYear = today->tm_year + 1900;

    long deadline = dateToDays(day, month, year);
    long current = dateToDays(
        currentDay, currentMonth, currentYear
    );

    return (int)(deadline - current);
}

// Refresh all task countdowns
void updateDeadlines(void) {
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed) {
            tasks[i].daysRemaining =
                calculateDaysRemaining(
                    tasks[i].dueDay,
                    tasks[i].dueMonth,
                    tasks[i].dueYear
                );
        }
    }
}
