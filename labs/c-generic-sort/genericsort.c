#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100000000
void *lineptr[MAX_LINES];

void quicksort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

int readlines(FILE *inputFile);

void writelines(FILE *output, int nlines);

int numcmp(char *, char *);

int readlines(FILE *inputFile) {
    char *line;

	int numLines = 0;

    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, inputFile) != -1)) {
		if (numLines == MAX_LINES) {
            printf("File too big to be sorted");
            exit(0);
        }
        lineptr[numLines] = malloc(strlen(line));
        strcpy(lineptr[numLines++], line);
    }

	fclose(inputFile);
    return numLines;
}

void writelines(FILE *output, int nlines) {
    for (int i = 0; i < nlines; i++) {
        fprintf(output, "%s", (char *)lineptr[i]);
    }
	fclose(output);
}

int numcmp(char *s1, char *s2) {
    double v1 = atof(s1),
        v2 = atof(s2);

    return (int) (v1 - v2);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Error: Invalid input.\n Example usage: ./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return -1;
	}

	int sortNumbers = 0;

	char inputFilename[50] = "",
		 outputFilename[50] = ""; 

	FILE *inputFile,
		 *outputFile;

	
	if (strcmp(argv[1], "-n") == 0) {
		if (argc != 6) {
			printf("Error: Invalid input.\n Example usage: ./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
			return -1;
		}
		sortNumbers = 1; // flag to know wether to sort by ints or strs
	} else if (argc != 5) {
		printf("Error: Invalid input.\n Example usage: ./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return -1;
	}

	strcat(inputFilename, argv[1+sortNumbers]); // input file name

	if (strcmp(argv[3+sortNumbers], "-o") != 0) {
		printf("Error: Invalid input.\n Example usage: ./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return -1;
	}

	strcat(outputFilename, argv[4+sortNumbers]); 

	if ((inputFile = fopen(inputFilename, "r")) == NULL) {
		printf("Error: file not found or cannot be open\n");
		return 0;
	}

	int numLines = readlines(inputFile);

	if (strcmp(argv[2+sortNumbers], "-quicksort") == 0) {
		quicksort(lineptr, 0, numLines-1, (sortNumbers ? (int (*)(void *, void *)) numcmp : (int (*)(void *, void *))strcmp));
	} else if (strcmp(argv[2+sortNumbers], "-mergesort") == 0) {
		mergesort(lineptr, 0, numLines-1, (sortNumbers ? (int (*)(void *, void *)) numcmp : (int (*)(void *, void *))strcmp));
	} else {
		printf("Error: sort [%s] is not supported\n", argv[2+sortNumbers]);
		return -1;
	}

    if ((outputFile = fopen(outputFilename, "w")) == NULL)
    {
        printf("Error: file cannot be created\n");
        return -1;
    }

    writelines(outputFile, numLines);
    printf("Results file can be found at ./%s\n", outputFilename);

    return 0;
}
