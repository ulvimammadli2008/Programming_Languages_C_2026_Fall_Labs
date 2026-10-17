// week5_task4_struct_binary_io.c
// Name: Ulvi  Surname: Mammadli  Student ID: 251ADB078
//
// Task 4: Binary file I/O
// Week 5 - Files & Modular Programming

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
    char name[MAX_NAME_LEN];
    int age;
    float gpa;
} Student;

int save_student_binary(const Student *s, const char *filename);
int load_student_binary(const char *filename, Student *out);
long file_size(const char *filename);

int main(void) {
    Student s1 = {0};
    Student loaded = {0};
    FILE *fp;
    const char *bin_file = "student.bin";
    const char *txt_file = "student.txt";
    long text_size;
    long binary_size;

    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    printf("Saving student in binary format...\n");

    if (save_student_binary(&s1, bin_file) != 0) {
        fprintf(stderr, "Error saving binary student data.\n");
        return EXIT_FAILURE;
    }

    /* Save the same record in text format for comparison. */
    fp = fopen(txt_file, "w");
    if (fp == NULL) {
        perror(txt_file);
        return EXIT_FAILURE;
    }

    if (fprintf(fp, "%s %d %.2f\n",
                s1.name, s1.age, s1.gpa) < 0) {
        fclose(fp);
        fprintf(stderr, "Error writing text student data.\n");
        return EXIT_FAILURE;
    }

    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing text student file.\n");
        return EXIT_FAILURE;
    }

    printf("Loading student from binary file...\n");

    if (load_student_binary(bin_file, &loaded) != 0) {
        fprintf(stderr, "Error loading binary student data.\n");
        return EXIT_FAILURE;
    }

    printf("Loaded student: %s, %d, GPA %.2f\n",
           loaded.name, loaded.age, loaded.gpa);

    text_size = file_size(txt_file);
    binary_size = file_size(bin_file);

    if (text_size < 0 || binary_size < 0) {
        fprintf(stderr, "Error determining file size.\n");
        return EXIT_FAILURE;
    }

    printf("Text file size: %ld bytes\n", text_size);
    printf("Binary file size: %ld bytes\n", binary_size);

    return EXIT_SUCCESS;
}

int save_student_binary(const Student *s, const char *filename) {
    FILE *fp;
    size_t written;

    if (s == NULL || filename == NULL) {
        return -1;
    }

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }

    written = fwrite(s, sizeof(Student), 1, fp);

    if (fclose(fp) == EOF || written != 1) {
        return -1;
    }

    return 0;
}

int load_student_binary(const char *filename, Student *out) {
    FILE *fp;
    size_t read_count;

    if (filename == NULL || out == NULL) {
        return -1;
    }

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }

    read_count = fread(out, sizeof(Student), 1, fp);

    if (fclose(fp) == EOF || read_count != 1) {
        return -1;
    }

    return 0;
}

long file_size(const char *filename) {
    FILE *fp;
    long size;

    if (filename == NULL) {
        return -1;
    }

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }

    size = ftell(fp);

    if (fclose(fp) == EOF) {
        return -1;
    }

    return size;
}