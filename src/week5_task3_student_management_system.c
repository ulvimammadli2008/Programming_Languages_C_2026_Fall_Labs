// week5_task3_student_management_system.c
// Name: Ulvi  Surname: Mammadli  Student ID: 251ADB078
//
// Task 3: Student management system with file persistence
// Week 5 - Files & Modular Programming

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

int load_students(Student arr[], int max);
int save_students(const Student arr[], int count);
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int read_line(const char *prompt, char *buf, size_t size);

static int only_whitespace(const char *text) {
    while (*text != '\0') {
        if (!isspace((unsigned char)*text)) {
            return 0;
        }
        text++;
    }

    return 1;
}

static char *skip_whitespace(char *text) {
    while (isspace((unsigned char)*text)) {
        text++;
    }

    return text;
}

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;

        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;
        } else {
            char *end;
            char *start = skip_whitespace(input);
            long value;

            errno = 0;
            value = strtol(start, &end, 10);

            while (isspace((unsigned char)*end)) {
                end++;
            }

            if (start == end || *end != '\0' || errno == ERANGE ||
                value < INT_MIN || value > INT_MAX) {
                choice = 0;
            } else {
                choice = (int)value;
            }
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;

            case 2:
                add_student(students, &count);
                break;

            case 3:
                if (save_students(students, count) != 0) {
                    fprintf(stderr, "Error saving student data.\n");
                    return EXIT_FAILURE;
                }

                printf("Data saved. Goodbye!\n");
                running = 0;
                break;

            default:
                printf("Invalid option. Try again.\n");
                break;
        }
    }

    return EXIT_SUCCESS;
}

int read_line(const char *prompt, char *buf, size_t size) {
    size_t len;

    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }

    len = strcspn(buf, "\r\n");

    if (buf[len] == '\0' && len == size - 1) {
        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }

    buf[len] = '\0';
    return 0;
}

int load_students(Student arr[], int max) {
    FILE *fp;
    char line[LINE_LEN];
    int count = 0;

    if (max <= 0 || arr == NULL) {
        return 0;
    }

    fp = fopen(DATA_FILE, "r");

    /* A missing file is normal on the first run. */
    if (fp == NULL) {
        return 0;
    }

    while (count < max && fgets(line, sizeof line, fp) != NULL) {
        Student student = {0};
        int consumed = 0;
        int fields;

        if (only_whitespace(line)) {
            continue;
        }

        fields = sscanf(line, "%49[^,],%d,%f %n",
                        student.name,
                        &student.id,
                        &student.gpa,
                        &consumed);

        if (fields != 3) {
            continue;
        }

        /* Reject trailing non-whitespace characters. */
        if (line[consumed] != '\0' ||
            only_whitespace(student.name) ||
            !isfinite(student.gpa)) {
            continue;
        }

        arr[count] = student;
        count++;
    }

    fclose(fp);
    return count;
}

int save_students(const Student arr[], int count) {
    FILE *fp;
    int i;

    if (count < 0 || (count > 0 && arr == NULL)) {
        return -1;
    }

    fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n",
                    arr[i].name, arr[i].id, arr[i].gpa) < 0) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) == EOF) {
        return -1;
    }

    return 0;
}

void add_student(Student arr[], int *count) {
    Student student = {0};
    char name[LINE_LEN];
    char id_input[LINE_LEN];
    char gpa_input[LINE_LEN];
    char *end;
    long id;
    float gpa;

    if (count == NULL || arr == NULL) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    if (read_line("Enter name: ", name, sizeof name) != 0) {
        return;
    }

    if (read_line("Enter ID: ", id_input, sizeof id_input) != 0) {
        return;
    }

    if (read_line("Enter GPA: ", gpa_input, sizeof gpa_input) != 0) {
        return;
    }

    /* Reject empty names, whitespace-only names, and commas. */
    if (name[0] == '\0' ||
        only_whitespace(name) ||
        strchr(name, ',') != NULL ||
        strlen(name) >= NAME_LEN) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    /* Validate the ID as an integer in the int range. */
    {
        char *start = skip_whitespace(id_input);

        errno = 0;
        id = strtol(start, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (start == end || *end != '\0' || errno == ERANGE ||
            id < INT_MIN || id > INT_MAX) {
            printf("Invalid input. Student not added.\n");
            return;
        }
    }

    /* Validate that GPA is numeric, finite, and between 0 and 4. */
    {
        char *start = skip_whitespace(gpa_input);

        errno = 0;
        gpa = strtof(start, &end);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (start == end || *end != '\0' || errno == ERANGE ||
            !isfinite(gpa) || gpa < 0.0f || gpa > 4.0f) {
            printf("Invalid input. Student not added.\n");
            return;
        }
    }

    strcpy(student.name, name);
    student.id = (int)id;
    student.gpa = gpa;

    arr[*count] = student;
    (*count)++;

    printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
    int i;

    if (count <= 0) {
        printf("No students found.\n");
        return;
    }

    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

    for (i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n",
               arr[i].id, arr[i].name, arr[i].gpa);
    }
}