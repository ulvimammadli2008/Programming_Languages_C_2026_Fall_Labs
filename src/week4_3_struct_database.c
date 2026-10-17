/*
 * week4_3_struct_database.c
 * Author: Ulvi Mammadli
 * Student ID: 251ADB078
 * Description:
 *   Implements an in-memory database using an array of structs.
 *   Dynamically allocates memory, reads student records,
 *   prints a table, and frees the allocated memory.
 */

#include <stdio.h>
#include <stdlib.h>

/* Define the Student structure. */
struct Student {
    char name[50];
    int id;
    float grade;
};

int main(void) {
    int n;
    struct Student *students = NULL;

    printf("Enter number of students: ");

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number.\n");
        return 1;
    }

    /* Allocate memory for n student records. */
    students = malloc(n * sizeof(struct Student));

    /* Check whether memory allocation succeeded. */
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Read each student's name, ID, and grade. */
    for (int i = 0; i < n; i++) {
        printf("Enter data for student %d: ", i + 1);

        if (scanf("%49s %d %f",
                  students[i].name,
                  &students[i].id,
                  &students[i].grade) != 3) {
            printf("Invalid input.\n");
            free(students);
            return 1;
        }
    }

    /* Print a blank line followed by the student table. */
    printf("\n");
    printf("%-6s %-11s %s\n", "ID", "Name", "Grade");

    for (int i = 0; i < n; i++) {
        printf("%-6d %-11s %.1f\n",
               students[i].id,
               students[i].name,
               students[i].grade);
    }

    /* Release the allocated memory. */
    free(students);

    return 0;
}