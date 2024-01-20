#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "aggregate.h"
#include "singular.h"

int main(int argc, char *argv[])
{

    // Variables to store the size and precision
    int size = 0;
    int precision = 0;
    bool hasSize = false;
    bool hasPrec = false;

    // Parse the command-line arguments
    // Index 0 holds program name, index 1 holds the operation name
    for (int i = 1; i < argc; i++)
    {

        // Check if first six chars match
        if (strncmp(argv[i], "-size=", 6) == 0)
        {
            size = atoi(argv[i] + 6); // Extract the size value after "-size="
            hasSize = true;
        }
        else if (strncmp(argv[i], "-prec=", 6) == 0)
        {
            precision = atoi(argv[i] + 6); // Extract the precision value after "-prec="
            hasPrec = true;
        }
    }

    // check if command line arguments are getting read, if not just stop

    int numRows = 0;
    double **jaggedArray = NULL;
    int *sizeOfRows = NULL;
    char *tokens[30];
    int numTokens = 0;

    char input[100];
    printf("Enter the jagged array: ");
    printf("Example: {5,4,54}{545,32,7}{7,6,9,23} ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character from the input, was giving output extra 0 at end for some reason
    input[strcspn(input, "\n")] = '\0';

    //                                                GENERATING JAGGED ARRAY

    char *token = strtok(input, "{}");
    while (token != NULL)
    {
        if (numRows >= 30)
        {
            fprintf(stderr, "Error: Maximum number of rows exceeded.\n");
            exit(EXIT_FAILURE);
        }

        jaggedArray = realloc(jaggedArray, (numRows + 1) * sizeof(double *));
        if (jaggedArray == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        sizeOfRows = realloc(sizeOfRows, (numRows + 1) * sizeof(int));
        if (sizeOfRows == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        tokens[numRows] = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(tokens[numRows], token);

        numRows++;

        token = strtok(NULL, "{}");
    }

    for (int i = 0; i < numRows; i++)
    {
        char *elementToken = strtok(tokens[i], ",");
        int rowSize = 0;
        double *row = NULL;
        while (elementToken != NULL)
        {
            row = realloc(row, (rowSize + 1) * sizeof(double));
            if (row == NULL)
            {
                fprintf(stderr, "Error: Memory allocation failed.\n");

                // Clean up allocated memory before exiting
                for (int j = 0; j < i; j++)
                {
                    free(jaggedArray[j]);
                    free(tokens[j]);
                }
                free(jaggedArray);
                free(sizeOfRows);
                exit(EXIT_FAILURE);
            }
            row[rowSize] = atof(elementToken);
            elementToken = strtok(NULL, ",");
            rowSize++;
        }
        sizeOfRows[i] = rowSize;
        jaggedArray[i] = row;
    }

    //                                                     TESTING
    printf("\n");
    printf("Jagged array:\n");
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < sizeOfRows[i]; j++)
        {
            printf("%.2lf ", jaggedArray[i][j]);
        }
        printf("\n");
    }

    if (size == 0)
    {
        size = sizeOfRows[0];
    }
    //                                                 FUNCTION CALLING

    // Creating array with proper lengths

    if (!hasSize)
    {
        size = sizeOfRows[0];
    }
    if (!hasPrec)
    {
        precision = 2;
    }

    double **jaggedArray2 = NULL; // Array that will store the values with proper sizing
    int numRows2 = 0;             // Number of rows in the new array
    int *cols = NULL;             // Will hold size of each row

    // Copy the elements from the original jagged array to the new array
    for (int i = 0; i < numRows; i++)
    {
        int rowSize = sizeOfRows[i];
        int numNewRows = (rowSize + size - 1) / size; // Calculate the number of new rows needed for the current row

        jaggedArray2 = realloc(jaggedArray2, (numRows2 + numNewRows) * sizeof(double *));
        cols = realloc(cols, (numRows2 + numNewRows) * sizeof(int));

        double *row = jaggedArray[i];
        int remainingElements = rowSize;
        int newRowCounter = 0;

        while (remainingElements > 0)
        {
            int newRowSize = (remainingElements >= size) ? size : remainingElements; // Calculate the size of the new row
            double *newRow = malloc(newRowSize * sizeof(double));                    // Allocate memory for the new row

            // Copy elements from the original row to the new row
            memcpy(newRow, row + (newRowCounter * size), newRowSize * sizeof(double));

            jaggedArray2[numRows2] = newRow;
            cols[numRows2] = newRowSize;

            newRowCounter++;
            remainingElements -= newRowSize;
            numRows2++;
        }
    }
    printf("\n");
    // Print the new jagged array
    printf("Jagged array 2:\n");
    for (int i = 0; i < numRows2; i++)
    {
        for (int j = 0; j < cols[i]; j++)
        {
            printf("%.*lf ", precision, jaggedArray2[i][j]);
        }
        printf("\n");
    }

    // Check the function name and perform the corresponding operation

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if ((strncmp(argv[i], "-size=", 6) != 0) && (strncmp(argv[i], "-prec=", 6) != 0))
            {

                // calls this aggregate function which will call all the specific ones needed
                aggregate(jaggedArray2, numRows2, &argv[i], cols, precision);

                if (strcmp(argv[i], "SHIFT") == 0)
                {
                    if (i + 1 < argc)
                    {
                        const double val = strtod(argv[i + 1], NULL);
                        for (int j = 0; j < numRows2; j++)
                        {
                            shift(jaggedArray2[j], cols[j], val, precision);
                            printf("\n");
                        }
                    }
                    else
                    {
                        fprintf(stderr, "No shift value specified.\n");
                        return 1;
                    }
                }
                else if (strcmp(argv[i], "PRINT") == 0)
                {
                    for (int j = 0; j < numRows2; j++)
                    {
                        print(jaggedArray2[j], cols[j], precision);
                        printf("\n");
                    }
                }
                else if (strcmp(argv[i], "FILTER") == 0)
                {
                    if (i + 2 < argc)
                    {
                        const char *filterType = argv[i + 1];
                        double threshold = atof(argv[i + 2]);

                        // Call the filterElements function with the appropriate arguments
                        for (int j = 0; j < numRows2; j++)
                        {
                            printf("Filtered Count:\n");
                            filter(jaggedArray2[j], cols[j], filterType, threshold, precision);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Insufficient filter arguments.\n");
                        return 1;
                    }
                }
            }
        }
    }

    return 0;

    // Freeing memory
    for (int i = 0; i < numRows; i++)
    {
        free(jaggedArray[i]);
        free(tokens[i]);
    }
    free(jaggedArray);
    free(sizeOfRows);
}
