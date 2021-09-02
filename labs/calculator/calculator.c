#include <stdio.h>
#include <string.h>
#include <stdlib.h>


long add(long a, long b) {
    return a + b;
}

long sub(long a, long b) {
    return a - b;
}

long mult(long a, long b) {
    return a * b;
}


// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    long result = values[0];
    long (*opFunc)(long, long);

    switch (operator)
    {
    case 1:
        opFunc = *add;
        break;
    case 2:
        opFunc = *sub;
        break;
    default:
        opFunc = *mult;
        break;
    }

    int currNum;
    for (int i=1; i<nValues; i++) {
        currNum = values[i];
        result = opFunc(result, currNum);
    }

    return result;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("A valid input was not provided.\n");
        printf("Usage example `./calculator.c add 1 2 3 4 5`\n");
        return -1;
    }

    char *op = argv[1];
    if (!(strcmp(op, "add") == 0 || strcmp(op, "sub") == 0 || strcmp(op, "mult") == 0)) {
        printf("A valid operator was not provided.\n");
        printf("Available operators: [add | sub | mult]\n");
        return -1;
    }

    int currNum,
        nums [argc -2];
    char *end;

    for (int i = 2; i < argc; i++) {
        currNum = strtol(argv[i], &end, 10);

        if (*end != '\0') {
            printf("The calculator can only work with integer values!\n");
            printf("Usage example `./calculator.c add 1 2 3 4 5`\n");
            return -1;
        }

        nums[i-2] = currNum;
    }

    int operator_code = 0;

    if (strcmp(op, "add") == 0) operator_code = 1;
    else if (strcmp(op, "sub") == 0) operator_code = 2;
    else operator_code = 3;

    long result = calc(operator_code, argc-2, nums);
    printf("%ld\n", result);

    return 0;
}
