#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mergesort(void *lineptr[], int left, int right,
			   int (*comp)(void *, void *));

void merge(void *lineptr[], int left, int right, int mid,
			   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
			   int (*comp)(void *, void *))
{
	if (right <= left)
		return;

	int mid = (left + right) / 2;
	mergesort(lineptr, left, mid, comp);
	mergesort(lineptr, mid + 1, right, comp);

	merge(lineptr, left, right, mid, comp);
}

void merge(void *lineptr[], int left, int right, int mid,
			   int (*comp)(void *, void *))
{
	int pLeft = left,
		pRight = mid + 1,
		k = left;
	
	void *temp[10000];

	while (pLeft <= mid && pRight <= right) {
		if ((*comp)(lineptr[pLeft], lineptr[pRight]) < 0) {
			temp[k] = malloc(strlen(lineptr[pLeft]));
			strcpy(temp[k++], lineptr[pLeft++]);
		}
		else {
			temp[k] = malloc(strlen(lineptr[pRight]));
			strcpy(temp[k++], lineptr[pRight++]);
		}
	}

	while (pLeft <= mid) {
		temp[k] = malloc(strlen(lineptr[pLeft]));
		strcpy(temp[k++], lineptr[pLeft++]);
	}

	while (pRight <= right) {
		temp[k] = malloc(strlen(lineptr[pRight]));
		strcpy(temp[k++], lineptr[pRight++]);
	}

    for (int i = left; i <= right; i++) {
        strcpy(lineptr[i], temp[i]);
    }
}


