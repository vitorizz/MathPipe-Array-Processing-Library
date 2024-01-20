#include "aggregate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

//                                        AGGREGATE OPERATIONS

// Function to count the number of elements in the jagged array
static int countElements(double arr[], int size)
{
    return size;
}

static double findMin(double arr[], int size)
{
    double min = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

static double findMax(double arr[], int size)
{
    double max = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

static double calculateSum(double arr[], int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}

static double calculateAverage(double arr[], int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum / size;
}

static double calculatePseudoAverage(double arr[], int size)
{
    double minimum = findMin(arr, size);
    double maximum = findMax(arr, size);
    return (minimum + maximum) / 2;
}

typedef int (*IntFunctionPtr)(double *, int);
typedef double (*DoubleFunctionPtr)(double *, int);

IntFunctionPtr intFunctions[] = {&countElements};
const char *intFuncNames[] = {"countElements"};

// Array of function pointers with return type double
DoubleFunctionPtr doubleFunctions[] = {&findMin, &findMax, &calculateSum, &calculateAverage, &calculatePseudoAverage};
const char *doubleFuncNames[] = {"findMin", "findMax", "calculateSum", "calculateAverage", "calculatePseudoAverage"};

void aggregate(double **jaggedArray2, int numRows2, char *argv[1], int *cols, int precision)
{
    if (strcmp(argv[1], "count") == 0)
    {
        for (int i = 0; i < numRows2; i++)
        {
            int count = intFunctions[0](jaggedArray2[i], cols[i]);
            printf("%d", count);
        }
    }
    else if (strcmp(argv[1], "min") == 0)
    {
        for (int i = 0; i < numRows2; i++)
        {
            double min = doubleFunctions[0](jaggedArray2[i], cols[i]);
            printf("Min: %.*lf\n", precision, min);
        }
    }
    else if (strcmp(argv[1], "max") == 0)
    {
        for (int i = 0; i < numRows2; i++)
        {
            double max = doubleFunctions[1](jaggedArray2[i], cols[i]);
            printf("Max: %.*lf\n", precision, max);
        }
    }
    else if (strcmp(argv[1], "sum") == 0)
    {
        for (int i = 0; i < numRows2; i++)
        {
            double sum = doubleFunctions[2](jaggedArray2[i], cols[i]);
            printf("Sum: %.*lf\n", precision, sum);
        }
    }
    else if (strcmp(argv[1], "avg") == 0)
    {
        for (int i = 0; i < numRows2; i++)
        {
            double average = doubleFunctions[3](jaggedArray2[i], cols[i]);
            printf("Average: %.*lf\n", precision, average);
        }
    }
    else if (strcmp(argv[1], "pavg") == 0)
    {
        for (int i = 0; i < numRows2; i++)
        {
            double pseudoAverage = doubleFunctions[4](jaggedArray2[i], cols[i]);
            printf("%.*lf\n", precision, pseudoAverage);
        }
    }
}
