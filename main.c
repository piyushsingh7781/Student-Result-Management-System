#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "records.dat"
#define SUBJECT_COUNT 5

typedef struct {
    int roll_no;
    char name[50];
    float marks[SUBJECT_COUNT];
    float total;
    float percentage;
    char grade;
} Student;

const char *SUBJECTS[SUBJECT_COUNT] = {
    "Mathematics",
    "Physics",
    "Computer Science",
    "English",
    "Electronics"
};

// Function prototypes
void calculateResult(Student *s);
int rollExists(int roll);
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void clearInputBuffer();

int main() {
    int choice;

    while (1) {
        printf("\n=========================================\n");
        printf("   STUDENT RESULT MANAGEMENT SYSTEM\n");
        printf("=========================================\n");
        printf("1. Add New Student Record\n");
        printf("2. Display All Student Results\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Update Student Marks\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n[Error] Invalid input. Please enter a number (1-6).\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("\nExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\n[Error] Invalid choice! Please select between 1 and 6.\n");
        }
    }
    return 0;
}

// Clear remaining characters in input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Calculate total, percentage, and assign grade
void calculateResult(Student *s) {
    s->total = 0.0f;
    for (int i = 0; i < SUBJECT_COUNT; i++) {
        s->total += s->marks[i];
    }
    s->percentage = s->total / SUBJECT_COUNT;

    if (s->percentage >= 90.0f) {
        s->grade = 'A';
    } else if (s->percentage >= 75.0f) {
        s->grade = 'B';
    } else if (s->percentage >= 60.0f) {
        s->grade = 'C';
    } else if (s->percentage >= 40.0f) {
        s->grade = 'D';
    } else {
        s->grade = 'F';
    }
}

// Check if roll number already exists in records.dat
int rollExists(int roll) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll_no == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// 1. Add a new student record
void addStudent() {
    Student s;
    printf("\n--- Add New Student ---\n");
    printf("Enter Roll Number: ");
    if (scanf("%d", &s.roll_no) != 1) {
        printf("[Error] Invalid Roll Number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (rollExists(s.roll_no)) {
        printf("[Error] Roll Number %d already exists!\n", s.roll_no);
        return;
    }

    printf("Enter Full Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // Remove trailing newline

    for (int i = 0; i < SUBJECT_COUNT; i++) {
        while (1) {
            printf("Enter marks for %s (0-100): ", SUBJECTS[i]);
            if (scanf("%f", &s.marks[i]) == 1 && s.marks[i] >= 0.0f && s.marks[i] <= 100.0f) {
                break;
            }
            printf("[Error] Please enter valid marks between 0 and 100.\n");
            clearInputBuffer();
        }
    }
    clearInputBuffer();

    calculateResult(&s);

    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("[Error] Unable to open file for writing.\n");
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\n[Success] Record added! Total: %.2f | Percentage: %.2f%% | Grade: %c\n",
           s.total, s.percentage, s.grade);
}

// 2. Display all student records
void displayAllStudents() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("\n[Info] No records found. Add a student first.\n");
        return;
    }

    Student s;
    int count = 0;

    printf("\n==========================================================================\n");
    printf("%-10s %-25s %-10s %-12s %-6s\n", "Roll No", "Name", "Total", "Percentage", "Grade");
    printf("==========================================================================\n");

    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("%-10d %-25s %-10.2f %-11.2f%% %-6c\n",
               s.roll_no, s.name, s.total, s.percentage, s.grade);
        count++;
    }

    printf("==========================================================================\n");
    printf("Total Records: %d\n", count);
    fclose(fp);
}

// 3. Search for a specific student and print report card
void searchStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("\n[Info] No records found.\n");
        return;
    }

    int roll, found = 0;
    printf("\nEnter Roll Number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("[Error] Invalid Roll Number.\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }
    clearInputBuffer();

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll_no == roll) {
            printf("\n-----------------------------------------\n");
            printf("             STUDENT REPORT CARD         \n");
            printf("-----------------------------------------\n");
            printf("Roll Number : %d\n", s.roll_no);
            printf("Name        : %s\n", s.name);
            printf("-----------------------------------------\n");
            for (int i = 0; i < SUBJECT_COUNT; i++) {
                printf("%-20s : %.2f / 100\n", SUBJECTS[i], s.marks[i]);
            }
            printf("-----------------------------------------\n");
            printf("Total Marks : %.2f / %.2f\n", s.total, SUBJECT_COUNT * 100.0f);
            printf("Percentage  : %.2f%%\n", s.percentage);
            printf("Grade       : %c (%s)\n", s.grade, s.grade == 'F' ? "FAIL" : "PASS");
            printf("-----------------------------------------\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\n[Info] No record found for Roll Number %d.\n", roll);
    }
    fclose(fp);
}

// 4. Update existing student marks in-place
void updateStudent() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("\n[Info] No records found.\n");
        return;
    }

    int roll, found = 0;
    printf("\nEnter Roll Number to update: ");
    if (scanf("%d", &roll) != 1) {
        printf("[Error] Invalid Roll Number.\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }
    clearInputBuffer();

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll_no == roll) {
            printf("\nUpdating marks for %s (Roll No: %d)\n", s.name, s.roll_no);
            for (int i = 0; i < SUBJECT_COUNT; i++) {
                while (1) {
                    printf("Enter new marks for %s (0-100): ", SUBJECTS[i]);
                    if (scanf("%f", &s.marks[i]) == 1 && s.marks[i] >= 0.0f && s.marks[i] <= 100.0f) {
                        break;
                    }
                    printf("[Error] Please enter valid marks between 0 and 100.\n");
                    clearInputBuffer();
                }
            }
            clearInputBuffer();

            calculateResult(&s);

            // Move file pointer back by one record size to overwrite
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            printf("\n[Success] Record updated! New Percentage: %.2f%% | Grade: %c\n",
                   s.percentage, s.grade);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\n[Info] No record found for Roll Number %d.\n", roll);
    }
    fclose(fp);
}

// 5. Delete a student record using temporary file swap
void deleteStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("\n[Info] No records found.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("[Error] Unable to create temporary file.\n");
        fclose(fp);
        return;
    }

    int roll, found = 0;
    printf("\nEnter Roll Number to delete: ");
    if (scanf("%d", &roll) != 1) {
        printf("[Error] Invalid Roll Number.\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("temp.dat");
        return;
    }
    clearInputBuffer();

    Student s;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll_no == roll) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("\n[Success] Record for Roll Number %d deleted.\n", roll);
    } else {
        remove("temp.dat");
        printf("\n[Info] No record found for Roll Number %d.\n", roll);
    }
}
