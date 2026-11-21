# 🎓 AcadWise

### Smart Academic Priority & Recovery System

**Plan Smarter. Prioritize Better. Meet Deadlines.**

![C](https://img.shields.io/badge/Language-C11-00599C?style=flat-square&logo=c)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-238636?style=flat-square)
![Interface](https://img.shields.io/badge/Interface-CLI-30363D?style=flat-square)
![Architecture](https://img.shields.io/badge/Architecture-Modular-8957E5?style=flat-square)

---

## About the Project

**AcadWise** is a terminal-based academic management system developed in C that helps students organize assignments, prioritize deadlines, manage study time, and recover from scheduling conflicts.

Unlike a traditional task manager, AcadWise evaluates whether academic workloads are achievable and recommends adjustments when students fall behind.

## Features

| Feature | Description |
|---|---|
| Task Management | Add, view, complete, update and delete assignments |
| Smart Priority Engine | Rank assignments by urgency, importance, difficulty and workload |
| Daily Study Scheduler | Allocate available study hours to pending tasks |
| Deadline Risk Analyzer | Detect workload shortfalls before upcoming deadlines |
| Academic Rescue Mode | Generate revised study schedules |
| Progress Dashboard | Track completion rates and subject-wise workloads |
| Automatic Countdown | Calculate remaining days using the system date |
| Persistent Storage | Save tasks locally with backup and recovery support |

## Terminal Preview

AcadWise provides an interactive, menu-driven command-line interface.

```text
==================================================
  ACADWISE v1.0
==================================================
  SMART ACADEMIC PRIORITY & RECOVERY

  TASK MANAGEMENT
  [1]  Add New Task
  [2]  View All Tasks
  [7]  Mark Task Completed
  [9]  Delete Task
  [10] Update Task Progress

  SMART PLANNING
  [3]  Smart Priority Ranking
  [4]  Daily Study Scheduler
  [5]  Deadline Risk Analyzer
  [6]  Academic Rescue Mode

  ANALYTICS
  [8]  Academic Progress Dashboard

  [0]  Save & Exit
==================================================
```

## How AcadWise Works

```text
       Academic Task Input
                |
                v
       Task Management
                |
                v
       Priority Calculation
                |
        +-------+-------+
        |               |
        v               v
   Study Planner   Risk Analyzer
        |               |
        +-------+-------+
                |
                v
        Recovery Planning
                |
                v
        Progress Dashboard
                |
                v
        Persistent Storage
```

### Priority Algorithm

AcadWise calculates task priority using a rule-based scoring formula:

**Priority = 4 × Urgency + 3 × Importance + 2 × Difficulty + Workload**

The workload component uses whole remaining hours capped at eight. Higher scores receive higher priority.

### Deadline Feasibility

The Risk Analyzer compares cumulative work due by each deadline against available study capacity.

**Available Capacity = Available Days × Daily Study Hours**

A positive workload shortfall indicates that a deadline may be at risk.

### Academic Rescue Mode

Rescue Mode uses an earliest-deadline-first approach to generate a revised schedule based on pending work and daily study availability.

It does not automatically detect missed sessions or modify actual task progress.

## Getting Started

### Requirements

- A C11-compatible compiler, such as GCC or Clang
- A terminal
- VS Code or another code editor (optional)

### Clone the Repository

```bash
git clone https://github.com/sarthaksuyal/AcadWise.git
cd AcadWise
```

### macOS / Linux

```bash
gcc -std=c11 -Wall -Wextra *.c -o acadwise
./acadwise
```

On macOS, `clang` can be used instead of `gcc`.

### Windows (MinGW-w64)

```powershell
gcc -std=c11 -Wall -Wextra *.c -o acadwise.exe
.\acadwise.exe
```

## Project Structure

| File | Responsibility |
|---|---|
| `main.c` | Application entry point and navigation |
| `AcadWise.h` | Shared structures and declarations |
| `tasks.c` | Academic task management |
| `input.c` | Input validation |
| `priority.c` | Priority calculation and ranking |
| `scheduler.c` | Daily study planning |
| `dates.c` | Date calculations and countdown |
| `risk.c` | Deadline feasibility analysis |
| `rescue.c` | Academic recovery planning |
| `dashboard.c` | Progress and workload statistics |
| `storage.c` | File persistence and backups |
| `ui.c` | Reusable terminal interface components |

## Data Storage

AcadWise stores task records in a local, versioned text file named `tasks.txt`.

The application supports automatic saving, permanent task IDs, and backup-based recovery.

Local task records, backup files, and compiled executables are excluded from Git through `.gitignore`.

## C Programming Concepts

Structures • Arrays • Strings • Functions • Loops • Conditional Statements • Sorting • File Handling • Pointers • Preprocessor Directives

## Current Limitations

- The scheduler assumes user-provided study availability.
- Rescue Mode replans remaining work but does not store historical study sessions.
- Scheduling is rule-based rather than AI-powered.
- Cross-platform source compatibility is intended; native platform testing should be completed.
- File backup recovery improves reliability but is not guaranteed to be atomic during unexpected interruptions.

## Future Improvements

- Graphical desktop interface
- Calendar integration
- Study-session history
- Custom daily availability
- Automatic schedule comparisons

---

**AcadWise** — Built with C, designed to make academic planning more manageable.