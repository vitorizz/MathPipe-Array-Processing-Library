#ifndef SINGULAR_H
#define SINGULAR_H

void print(double *arr, int size, int precision);

void filter(double *array, int numCols, const char *filterType, double threshold, int precision);

void shift(double *array, int numCols, double value, int precision);

#endif