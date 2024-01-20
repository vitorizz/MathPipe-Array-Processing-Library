#include "singular.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

void print(double *arr, int size, int precision)
{
    for (int i = 0; i < size; i++)
    {
        printf("%.*lf", precision, arr[i]);
    }
}

void filter(double *array, int numCols, const char *filterType, double threshold, int precision)
{
    for (int i = 0; i < numCols; i++)
    {
        double number = array[i];
        int shouldPrint = 0;

        if (strcmp(filterType, "EQ") == 0 && number == threshold)
        {
            shouldPrint = 1;
        }
        else if (strcmp(filterType, "NEQ") == 0 && number != threshold)
        {
            shouldPrint = 1;
        }
        else if (strcmp(filterType, "GEQ") == 0 && number >= threshold)
        {
            shouldPrint = 1;
        }
        else if (strcmp(filterType, "LEQ") == 0 && number <= threshold)
        {
            shouldPrint = 1;
        }
        else if (strcmp(filterType, "LESS") == 0 && number < threshold)
        {
            shouldPrint = 1;
        }
        else if (strcmp(filterType, "GREATER") == 0 && number > threshold)
        {
            shouldPrint = 1;
        }

        if (shouldPrint)
        {
            printf("%.*lf\n", precision, number);
        }
    }
}

void shift(double *array, int numCols, double value, int precision)
{
    for (int i = 0; i < numCols; i++)
    {
        array[i] += value;
    }
    for (int i = 0; i < numCols; i++)
    {
        printf("%.*lf", precision, array[i]);
    }
}