//
// Here is a refactored version of the algorithm
// for more comfortable investigation
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 1024

char* regex_divisible_by(int);
char*** createMatrix(int);
void populateMatrix(char***, int);
void processMatrix(char***, int, char**, char**);
char* concatenate(const char*, const char*);
char* generateResult(char***, int, int);


int main(int argc, char** argv) {
    /* Checking the count of arguments and
        if the provided argument is a positive integer */
    if(argc != 2 || atoi(argv[1]) <= 0) {
        printf("Usage: %s positive_integer\n", argv[0]);
        return 1;
    }

    /* A function call to get the regex
        for numbers divisible by the provided number */
    char* regex = regex_divisible_by(atoi(argv[1]));
    printf("%d: %s\n", atoi(argv[1]), regex);

    free(regex);

    return 0;
}

// Function to generate a regex for numbers divisible by a given number
char* regex_divisible_by(int n) {
    int exponent = 0;
    char*** matrix = createMatrix(n);
    char* bufInput[n];
    char* bufOutput[n];

    // Validation of the input
    assert(n > 0);

    if (n == 1) {
        char* result = (char*)malloc(sizeof(char));
        strcat(result, "^(0|1)+$");
        return result;
    }

    // Getting the power of 2 in the factorization of n
    while (n % 2 == 0) {
        n /= 2;
        exponent++;
    }

    // Filling the matrix with initial values
    populateMatrix(matrix, n);

    // Processing the matrix
    processMatrix(matrix, n, bufInput, bufOutput);

    // Generating the final regex
    return generateResult(matrix, n, exponent);
}

/* Helper function to allocate memory
    for the matrix and initialize it with empty strings */
char*** createMatrix(int n) {
    char*** matrix = malloc(n * sizeof(char**));

    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(char*));

        for (int j = 0; j < n; j++) {
            matrix[i][j] = (char*)calloc(1, sizeof(char));
        }
    }

    return matrix;
}

// An additional function to fill the matrix with initial values
void populateMatrix(char*** matrix, int n) {
    for(int i = 0; i < n; i++) {
        matrix[i][i * 2 % n] = malloc(2);
        strcpy(matrix[i][i * 2 % n], "0");

        matrix[i][(i * 2 + 1) % n] = malloc(2);
        strcpy(matrix[i][(i * 2 + 1) % n], "1");
    }

    /* Special cases:
        If n is odd, we need to adjust the matrix by adding
        the string "01*0" to the cell that corresponds to the
        middle row and the second-to-last column. */
    if(n % 2 == 1) {
        matrix[(n - 1) / 2][n - 2] = malloc(5);
        strcpy(matrix[(n - 1) / 2][n - 2], "01*0");

    /* If n is even, we add the string "11*0" to the cell
        corresponding to the row before the middle one and
        the second-to-last column. */
    } else {
        matrix[n / 2 - 1][n - 2] = malloc(5);
        strcpy(matrix[n / 2 - 1][n - 2], "11*0");
    }
}

// A function to process the matrix
void processMatrix(char*** matrix, int n, char** bufInput, char** bufOutput) {
    for(int step = 1; step < n - 1; step++) {
        for(int i = 0; i < n; i++) {
            bufInput[i] = "";
            bufOutput[i] = "";
        }

        /* Copying 'step'-th row and column values from the matrix
            to input and output buffers, and clearing these matrix
            cells for future processing. */
        for(int i = 0; i < n; i++) {
            if(strcmp(matrix[i][step], "") != 0) {
                bufInput[i] = matrix[i][step];
                matrix[i][step] = "";
            }

            if(strcmp(matrix[step][i], "") != 0) {
                bufOutput[i] = matrix[step][i];
                matrix[step][i] = "";
            }
        }

        // Updating the matrix based on the input and output buffers
        for(int i = 0; i < n; i++) {
            if(strcmp(bufInput[i], "") == 0) continue;

            for(int j = 0; j < n; j++) {
                if(strcmp(bufOutput[j], "") == 0) continue;

                /* If the input buffer for the current step is empty, we're not in a self-loop state,
                    which means the case when we're not dealing with a situation where a single state in
                    the automaton is transitioning to itself. In this case, concatenate the buffers of
                    the current row and column. If the matrix cell at [i][j] is already populated,
                    append the new combination with a "|". */
                if(strcmp(bufInput[step], "") == 0) {
                    if(strcmp(matrix[i][j], "") == 0)
                        matrix[i][j] = concatenate(concatenate("(", bufInput[i]), concatenate(")(", concatenate(bufOutput[j], ")")));
                    else
                        matrix[i][j] = concatenate(concatenate("(", matrix[i][j]), concatenate(")|(", concatenate(bufInput[i], concatenate(")(", concatenate(bufOutput[j], ")")))));

                /* If the step is the current row, we're in a self-loop state.
                    The current row's input buffer is combined with the current
                    step's input buffer and current column's output buffer.
                    If [i][j] is populated, append the new combination with a "|". */
                } else if(i != step) {
                    if(strcmp(matrix[i][j], "") == 0)
                        matrix[i][j] = concatenate(concatenate("(", bufInput[i]), concatenate(")(", concatenate(bufInput[step], concatenate(")*(", concatenate(bufOutput[j], ")")))));
                    else
                        matrix[i][j] = concatenate(concatenate("(", matrix[i][j]), concatenate(")|(", concatenate(concatenate("(", bufInput[i]), concatenate(")(", concatenate(bufInput[step], concatenate(")*(", concatenate(bufOutput[j], "))")))))));
                }
            }
        }
    }
}

// Function to concatenate two strings
char* concatenate(const char* str1, const char* str2) {
    char* result = malloc(strlen(str1) + strlen(str2) + 1);

    if(result == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

// A function to generate the final regex
char* generateResult(char*** matrix, int n, int exponent) {
    char* result = (char*)calloc(135001, sizeof(char));

    /* Handling the case when 'n' is not divisible by 2 (i.e., it's an odd number).
        We are creating a regular expression that matches any combination of
        the pattern in 'matrix[0][0]' */
    if (exponent == 0) {
        strncat(result, "^(", 135000);
        strncat(result, matrix[0][0], 135000 - strlen(result));
        strncat(result, ")+$", 135000 - strlen(result));

    /* Handling the case when 'n' is 1.
        In this case, the regular expression accepts any sequence of 0s and 1s,
        but it must end with a number of zeros equal to 'exponent' */
    } else if(n == 1) {
        strncat(result, "^(0|1)*", 1023);

        for(int i = 0; i < exponent; i++)
            strncat(result, "0", 1023 - strlen(result));

        strncat(result, "$", 1023 - strlen(result));

    /* Handling the general case (when 'n' is divisible by 2 and 'n' is not 1).
        Here, the regular expression accepts any sequence of the pattern in 'matrix[0][0]'
        ending with a number of zeros equal to 'exponent' */
    } else {
        strncat(result, "^(", 1023);
        strncat(result, matrix[0][0], 1023 - strlen(result));
        strncat(result, ")*", 1023 - strlen(result));

        for(int i = 0; i < exponent; i++)
            strncat(result, "0", 1023 - strlen(result));

        strncat(result, "$", 1023 - strlen(result));
    }

    return result;
}
