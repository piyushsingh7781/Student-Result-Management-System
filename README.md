<div align="center">

# 🎓 Student Result Management System (SRMS)

**A complete, file-based academic record management and automated grading system built in C.**

![Language](https://shields.io)
![Storage](https://shields.io)
![Platform](https://shields.io)
![Project Type](https://shields.io)

</div>

---

## 📑 Table of Contents

1. [Abstract](#-1-abstract)
2. [Introduction & Problem Statement](#-2-introduction--problem-statement)
3. [Objectives](#-3-objectives)
4. [Key Features](#-4-key-features)
5. [System Architecture & Design](#-5-system-architecture--design)
6. [Data Structure & Grading Logic](#-6-data-structure--grading-logic)
7. [Core Algorithms](#-7-core-algorithms)
8. [Installation & Execution](#-8-installation--execution)
9. [Sample Console Output](#-9-sample-console-output)
10. [Software Testing & Validation](#-10-software-testing--validation)
11. [Repository Structure](#-11-repository-structure)
12. [Conclusion & Future Scope](#-12-conclusion--future-scope)

---

## 📖 1. Abstract

The **Student Result Management System (SRMS)** is a console-based software application developed in the C programming language to automate the storage, calculation, and retrieval of student academic performance. Traditional manual grading workflows rely on physical ledgers and spreadsheets, which are time-consuming, prone to human arithmetic errors, and difficult to audit. This system replaces manual record-keeping with a structured, binary file-based engine (`records.dat`). Leveraging C structures (`struct`) and direct file pointer manipulation, SRMS performs reliable Create, Read, Update, and Delete (CRUD) operations, validates inputs, computes totals, percentages, and letter grades automatically, and generates formatted report cards on demand.

---

## 🔍 2. Introduction & Problem Statement

### Existing Manual System
In traditional setups, examination records are compiled manually on paper sheets, totals and percentages are calculated by hand, and report cards are written individually.

### Limitations of the Manual Approach
- **High Error Rate:** Manual arithmetic frequently introduces calculation mistakes in totals and grade boundaries.
- **Slow Retrieval:** Locating an individual student's record across physical registers is inefficient.
- **Data Vulnerability:** Physical files are susceptible to damage, loss, and duplication.
- **Difficult Updates:** Correcting a single subject mark requires recalculating the entire marksheet manually.

### Proposed Solution
SRMS automates the entire result lifecycle. Once subject marks are entered, the system validates input bounds, blocks duplicate Roll Numbers, calculates percentages and grades deterministically, and serializes the record to a persistent binary file for instant retrieval.

---

## 🎯 3. Objectives

1. **Automate Grading:** Eliminate manual computation of total marks, percentages, and letter grades.
2. **Ensure Data Persistence:** Store records permanently on disk via binary file streams (`records.dat`).
3. **Enforce Data Integrity:** Prevent duplicate primary keys (Roll Numbers) and reject invalid marks (`< 0` or `> 100`).
4. **Optimize Storage Operations:** Support in-place record updates (`fseek`) and safe deletions via atomic file swapping.
5. **Standardize Reporting:** Produce uniform tabular class summaries and detailed individual student marksheets.

---

## ✨ 4. Key Features

- **Add Student Records:** Registers students with a unique Roll Number, Full Name, and marks across 5 subjects.
- **Automated Grade Evaluation:** Computes total marks, overall percentage, pass/fail status, and assigns letter grades (`A`–`F`).
- **Display All Results:** Renders an aligned summary table of all stored students and total record count.
- **Search Individual Report Card:** Queries by Roll Number to generate a complete subject-wise transcript.
- **In-Place Record Modification:** Updates subject marks for existing students and recalculates their final grade automatically.
- **Safe Record Deletion:** Removes records cleanly using temporary file replacement without corrupting existing data.
- **Input Buffer Sanitization:** Prevents infinite loops caused by invalid non-numeric inputs in `stdin`.

---

## 🏗️ 5. System Architecture & Design

### Modular Layer Breakdown
The codebase follows a clean procedural architecture divided into four logical layers:

1. **Presentation Layer (`main`, `displayAllStudents`, `searchStudent`):** Manages the interactive CLI menu and renders aligned ASCII tables and report cards.
2. **Validation Layer (`rollExists`, `clearInputBuffer`):** Sanitizes standard input (`stdin`) and enforces primary key uniqueness.
3. **Business Logic Layer (`calculateResult`):** Decouples percentage and grading formulas from file I/O and UI code.
4. **Persistence Layer (`addStudent`, `updateStudent`, `deleteStudent`):** Handles low-level binary file operations (`fopen`, `fread`, `fwrite`, `fseek`, `remove`, `rename`).

### Data Flow Diagram (DFD)

```text
+---------------+   Student Details & Marks    +-----------------------+

|               | ---------------------------> |                       |
| Admin / User  |                              |   SRMS Core Engine    |
|               | <--------------------------- |       (main.c)        |
+---------------+   Formatted Report Cards     +-----------------------+
                                                           ^
                                                           | Binary Read/Write
                                                           v
                                               +-----------------------+

                                               |      records.dat      |
                                               |   (Binary Storage)    |
                                               +-----------------------+
```

---

## 📊 6. Data Structure & Grading Logic

### Core Data Model (`Student` Struct)
Each student record is stored as a contiguous memory block written directly to `records.dat`:

```c
typedef struct {
    int roll_no;             // Unique Primary Key (4 bytes)
    char name;           // Student Full Name (50 bytes)
    float marks;          // Marks in 5 Subjects (20 bytes)
    float total;             // Auto-computed Sum (4 bytes)
    float percentage;        // Auto-computed Percentage (4 bytes)
    char grade;              // Auto-assigned Grade 'A'-'F' (1 byte)
} Student;
```

### Grading Scale

| Percentage Range | Letter Grade | Academic Status |
| :--- | :---: | :---: |
| **90.00% – 100.00%** | `A` | ✅ PASS |
| **75.00% – 89.99%** | `B` | ✅ PASS |
| **60.00% – 74.99%** | `C` | ✅ PASS |
| **40.00% – 59.99%** | `D` | ✅ PASS |
| **Below 40.00%** | `F` | ❌ FAIL |

---

## ⚙️ 7. Core Algorithms

### In-Place Update (`fseek` Strategy)
1. Open `records.dat` in `"rb+"` (read/write binary) mode.
2. Read `Student` records sequentially until `s.roll_no == target_roll`.
3. Move the file cursor backward by one record size: `fseek(fp, -(long)sizeof(Student), SEEK_CUR)`.
4. Overwrite the updated `Student` struct directly in place using `fwrite()`.

### Safe Deletion (Temporary File Strategy)
1. Open `records.dat` in `"rb"` mode and create `temp.dat` in `"wb"` mode.
2. Copy every record *except* the matching `roll_no` into `temp.dat`.
3. Close both file pointers, delete `records.dat` using `remove()`, and rename `temp.dat` to `records.dat` using `rename()`.

---

## 🚀 8. Installation & Execution

### Prerequisites
- A C compiler such as **GCC** (MinGW on Windows, GCC on Linux, or Clang/Xcode Command Line Tools on macOS).

### Step-by-Step Setup

1. **Clone the repository:**
   ```bash
   git clone https://github.com
   cd student-result-management-c
   ```

2. **Compile the source code:**
   ```bash
   gcc -Wall -Wextra main.c -o srms
   ```

3. **Run the executable:**
   - **Linux / macOS:**
     ```bash
     ./srms
     ```
   - **Windows:**
     ```bash
     srms.exe
     ```

---

## 🖥️ 9. Sample Console Output

```text
=========================================
   STUDENT RESULT MANAGEMENT SYSTEM
=========================================
1. Add New Student Record
2. Display All Student Results
3. Search Student by Roll Number
4. Update Student Marks
5. Delete Student Record
6. Exit
-----------------------------------------
Enter your choice: 3

Enter Roll Number to search: 101

-----------------------------------------
             STUDENT REPORT CARD         
-----------------------------------------
Roll Number : 101
Name        : Aarav Sharma
-----------------------------------------
Mathematics          : 92.00 / 100
Physics              : 88.00 / 100
Computer Science     : 95.00 / 100
English              : 84.00 / 100
Electronics          : 90.00 / 100
-----------------------------------------
Total Marks : 449.00 / 500.00
Percentage  : 89.80%
Grade       : B (PASS)
-----------------------------------------
```

---

## 🧪 10. Software Testing & Validation

| Test ID | Scenario | Input Provided | Expected Behavior | Result |
| :--- | :--- | :--- | :--- | :---: |
| **TC-01** | Add valid student | Roll: `101`, Marks: `85, 90, 88, 76, 91` | Saves record; Percentage: `86.00%`, Grade: `B` | ✅ Pass |
| **TC-02** | Duplicate Roll Number | Roll: `101` (already exists) | Rejects entry with duplicate roll error message | ✅ Pass |
| **TC-03** | Out-of-bounds marks | Marks: `105` or `-5` | Prompts user again to enter marks in `0–100` | ✅ Pass |
| **TC-04** | Non-numeric menu input | Choice: `"xyz"` | Clears `stdin` buffer safely without crashing | ✅ Pass |
