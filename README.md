# AcadWise

**Smart Academic Priority & Recovery System**  
*Plan smarter. Prioritize better. Meet deadlines.*

AcadWise is a modular, command-line academic workload planner written in **C11**. It helps students record assignments, rank work by urgency and importance, plan study sessions, evaluate deadline feasibility, and create revised schedules when they fall behind.

> **Project status:** Functional C prototype. This is a rule-based planner, not an AI system. Scheduling suggestions depend on the accuracy of entered estimates and availability.

## The problem

Students often manage several assignments, lab records, projects and quizzes with overlapping deadlines. A basic to-do list shows what is due but does not help assess whether the workload fits the time available. AcadWise combines task tracking with transparent priority and scheduling calculations.

## Features

| Menu | Feature | Description |
|---|---|---|
| 1 | Add task | Record a title, subject, deadline, importance, difficulty and estimated effort. |
| 2 | View tasks | Show task details, countdowns, progress and completion status. |
| 3 | Priority ranking | Score and rank pending tasks using a rule-based heuristic. |
| 4 | Daily study planner | Allocate today's available study hours to pending tasks in priority order, including partial allocations. |
| 5 | Deadline risk analyzer | Compare cumulative work due by each deadline against estimated study capacity. |
| 6 | Academic Rescue Mode | Propose a revised, earliest-deadline-first schedule from remaining work. |
| 7 | Complete task | Mark an assignment finished. |
| 8 | Progress dashboard | Summarize completion, urgent and overdue tasks, remaining hours and subject workloads. |
| 9 | Delete task | Remove a selected task after confirmation. |
| 10 | Update progress | Record hours worked and update remaining effort. |
| 0 | Save and exit | Save tasks and close the program. |

Additional capabilities include input validation, date-based countdowns, local file persistence, a reusable terminal UI, and persistent task IDs.

## How the algorithms work

### Priority score

The priority engine uses:

`P = 4U + 3I + 2D + H`

- `U`: urgency based on days until deadline (10 if due today/overdue; 9 if due tomorrow; 7 within three days; 4 within seven days; otherwise 1).
- `I`: importance from 1 to 5.
- `D`: difficulty from 1 to 5.
- `H`: whole remaining hours, capped at 8.

Higher scores are ranked first. This is a custom heuristic, not a guaranteed optimal schedule.

### Daily study planner

Pending tasks are sorted by priority. Available hours are allocated to each task in turn; a task can receive a partial allocation. **Generating a plan does not mark work as completed.** Actual progress is recorded separately.

### Deadline risk analyzer

Pending tasks are ordered by deadline and grouped when they share a date. The analyzer compares cumulative work due by a checkpoint with `(days_remaining + 1) * daily_study_hours`. The estimate assumes study time is available today and on deadline days, and that tasks can be split into smaller sessions.

### Academic Rescue Mode

Rescue Mode produces a revised day-by-day schedule using current remaining workload, earliest deadlines, and daily study availability. It reports unscheduled work and distinguishes tasks outside its 30-day planning horizon. It **does not** automatically detect missed historical sessions or modify actual task progress.

## Project structure

```text
AcadWise/
├── main.c          # Menu and application entry point
├── AcadWise.h      # Shared Task structure and declarations
├── tasks.c         # Add, view, complete, delete, progress
├── input.c         # Input validation
├── dates.c         # Calendar validation and countdowns
├── priority.c      # Priority scoring and ranking
├── scheduler.c     # Daily study allocation
├── risk.c          # Deadline feasibility checks
├── rescue.c        # Revised recovery scheduling
├── dashboard.c     # Progress and workload summaries
├── storage.c       # Local persistence and backup handling
├── ui.c            # Terminal presentation helpers
├── README.md       # This guide
└── .gitignore      # Local files excluded from Git
```

## Requirements

- A C11-compatible compiler (GCC or Clang)
- A terminal/command prompt
- No external C libraries required for the CLI application

## Build and run

Open a terminal **inside the project directory**.

**macOS (Clang):**

```bash
clang -std=c11 -Wall -Wextra -Wpedantic *.c -o acadwise
./acadwise
```

**Linux (GCC):**

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic *.c -o acadwise
./acadwise
```

**Windows (PowerShell with MinGW-w64 GCC installed):**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic (Get-ChildItem -Filter *.c).Name -o acadwise.exe
.\acadwise.exe
```

Compile again after editing source files. The source is intended to be cross-platform; builds on all target operating systems should be verified separately.

## Quick demonstration

1. Add a programming assignment due tomorrow, requiring 4 hours.
2. Add a mathematics task due in three days, requiring 2 hours.
3. Open **Priority Ranking** and explain the scores.
4. Generate a **Daily Study Plan** with 3 available hours.
5. Record 1 hour of actual progress and inspect the updated workload.
6. Run the **Deadline Risk Analyzer** with limited daily availability.
7. Run **Academic Rescue Mode** and compare the revised schedule.
8. Open the **Progress Dashboard**, then save and restart to verify persistence.

## Data storage and privacy

AcadWise saves task data locally in `tasks.txt` (a versioned text format). It may also create backup, temporary, and legacy-import files. These files are **ignored by Git** so teammates do not accidentally publish personal task records. Each clone starts with its own local dataset. Keep a backup before testing recovery or changing storage code.

## Known limitations

- Study time is estimated; real completion speed may differ.
- Risk analysis assumes a constant daily study capacity and includes deadline days.
- Rescue Mode replans current work but does not track historical missed sessions.
- The recovery planner considers a maximum 30-day horizon.
- The terminal UI is not a graphical desktop application.
- File backup/replacement is designed to improve recoverability but is not guaranteed atomic across all platforms.

## C concepts demonstrated

Structures, arrays, strings, functions, modular programming, header files, conditionals, loops, searching, sorting, formatted input/output, file handling, and date calculations.

## Team and academic context

Developed as a **C Programming Mini Hackathon** project by a three-member team. Add the confirmed team name, member names, batch/section, and faculty details here before final submission.

## License

No license has been selected yet. Until a license is added, standard copyright rules apply; publishing the code does not automatically grant reuse permissions.
