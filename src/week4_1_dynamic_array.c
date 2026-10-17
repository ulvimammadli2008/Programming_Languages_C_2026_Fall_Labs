/*
 * week4_1_dynamic_array.c
 * Author: Ulvi Mammadli
 * Student ID: 251ADB078
 * Description:
 *   Demonstrates dynamic array allocation using malloc.
 *   Reads integers, calculates their sum and average,
 *   and frees allocated memory.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    int *arr = NULL;
    int sum = 0;
    double average;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size.\n");
        return 1;
    }

    /* Allocate memory for n integers. */
    arr = malloc(n * sizeof(int));

    /* Check whether memory allocation succeeded. */
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Read the integers into the allocated array. */
    printf("Enter %d integers: ", n);

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input.\n");
            free(arr);
            return 1;
        }
    }

    /* Calculate the sum of the integers. */
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    /* Calculate the average using floating-point division. */
    average = (double)sum / n;

    printf("Sum = %d\n", sum);
    printf("Average = %.2f\n", average);

    /* Release the allocated memory. */
    free(arr);

    return 0;
}