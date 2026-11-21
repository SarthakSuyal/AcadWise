
<div align="center">

# AcadWise

### Smart Academic Priority & Recovery System

**Plan Smarter. Prioritize Better. Meet Deadlines.**

<br>

<img src="https://img.shields.io/badge/Language-C11-00599C?style=for-the-badge&logo=c" alt="C11">
&nbsp;&nbsp;
<img src="https://img.shields.io/badge/Interface-CLI-30363D?style=for-the-badge" alt="CLI">

<br>

<img src="https://img.shields.io/badge/Architecture-Modular-8957E5?style=for-the-badge" alt="Modular">
&nbsp;&nbsp;
<img src="https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-238636?style=for-the-badge" alt="Platforms">

<br><br>

*A modular academic planning application developed in C.*

</div>

---

## Overview

Students often struggle with overlapping assignments, limited study hours, and approaching deadlines.

**AcadWise** addresses this problem through a menu-driven C application that helps students organize assignments, calculate priorities, generate study plans, identify deadline risks, and replan unfinished work.

The objective is to make academic workload management more structured using fundamental C programming concepts.

## Features

| Feature | Functionality |
|---|---|
| Task Management | Add, view, complete, update, and delete assignments |
| Priority Engine | Rank tasks using urgency, importance, difficulty, and workload |
| Study Scheduler | Allocate available study hours |
| Deadline Risk Analyzer | Identify deadlines with insufficient study capacity |
| Academic Rescue Mode | Generate revised deadline-aware schedules |
| Progress Dashboard | Track completion and subject-wise workload |
| Automatic Countdown | Calculate days remaining from the system date |
| Persistent Storage | Save assignments with backup and recovery support |

## System Architecture

```text
                 ACADWISE
                    |
             Task Management
                    |
          +---------+---------+
          |                   |
    Priority Engine      Deadline Analyzer
          |                   |
    Study Scheduler           |
          |                   |
          +---------+---------+
                    |
              Rescue Mode
                    |
            Progress Dashboard

      Shared Data: Task Structures
      Persistence: File Storage
```

## Core Logic

**Priority Calculation**

```text
Priority = 4(Urgency) + 3(Importance)
         + 2(Difficulty) + Workload
```

Workload uses whole remaining hours capped at eight. Higher scores receive higher priority.

**Deadline Risk Analysis**

```text
Available Capacity = Available Days x Daily Hours

Shortfall = Cumulative Work - Available Capacity
```

A positive shortfall indicates a potential deadline conflict.

**Academic Rescue Mode**

Pending tasks are arranged by earliest deadline and allocated available study hours. The system reports overdue and unscheduled work without changing actual task progress.

## Installation & Execution

**Requirements:** C11-compatible compiler (GCC or Clang).

Clone the repository:

```bash
git clone https://github.com/sarthaksuyal/AcadWise.git
cd AcadWise
```

**macOS / Linux**

```bash
gcc -std=c11 -Wall -Wextra *.c -o acadwise
./acadwise
```

On macOS, Clang may be used instead of GCC.

**Windows (MinGW-w64)**

```powershell
gcc -std=c11 -Wall -Wextra *.c -o acadwise.exe
.\acadwise.exe
```

## Project Structure

| Module | Responsibility |
|---|---|
| `main.c` | Program navigation |
| `AcadWise.h` | Shared structures and declarations |
| `tasks.c` / `input.c` | Task management and input validation |
| `priority.c` / `scheduler.c` | Priority ranking and study planning |
| `dates.c` / `risk.c` | Deadline calculations and risk analysis |
| `rescue.c` | Academic recovery planning |
| `dashboard.c` | Progress statistics |
| `storage.c` | Persistent file storage |
| `ui.c` | Terminal interface |

## Testing

Sample test cases for the working program:

| Test Case | Expected Output | Actual Output |
|---|---|---|
| Add a valid task | Task created | To be verified |
| Enter invalid date | Input rejected | To be verified |
| Update task progress | Remaining hours reduced | To be verified |
| Insufficient study hours | Deadline warning | To be verified |
| Restart application | Saved tasks restored | To be verified |

*Replace the final column with observed results from the final tested build.*

## C Concepts Used

Structures, arrays, strings, pointers, functions, loops, conditionals, sorting, file handling, and preprocessor directives.

## Team Details

**Mini Hackathon — C Programming**

| Detail | Information |
|---|---|
| Team Number | To be added |
| Batch | To be added |
| Member 1 | Sarthak Suyal |
| Member 2 | To be added |
| Member 3 | To be added |

## Future Scope

GUI support, calendar integration, custom daily availability, and study-session history.

---

<div align="center">

**AcadWise — Built with C. Designed for Smarter Academic Planning.**

</div>
