// week5_task1_file_io.c
// Name: Ulvi  Surname: Mammadli  Student ID: 251ADB078
//
// Task 1: Read and write data from text files
// Week 5 - Files & Modular Programming

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_FILENAME "data.txt"
#define LINE_LEN 256

int main(int argc, char *argv[]) {
    FILE *fp;
    const char *filename = DEFAULT_FILENAME;
    char line[LINE_LEN];
    int line_count = 0;

    if (argc > 1) {
        filename = argv[1];
    }

    printf("Writing lines to %s...\n", filename);

    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    if (fprintf(fp, "Hello, file I/O in C!\n") < 0 ||
        fprintf(fp, "This is another line.\n") < 0) {
        perror(filename);
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (fclose(fp) == EOF) {
        perror(filename);
        return EXIT_FAILURE;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    printf("Reading contents:\n");

    while (fgets(line, sizeof line, fp) != NULL) {
        printf("%s", line);
        line_count++;
    }

    if (ferror(fp)) {
        perror(filename);
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (fclose(fp) == EOF) {
        perror(filename);
        return EXIT_FAILURE;
    }

    printf("Lines read: %d\n", line_count);

    return EXIT_SUCCESS;
}