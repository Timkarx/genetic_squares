#include <stdio.h>
#include <stdlib.h>

// function to print array elements
void printArray(unsigned char array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf("%u  ", array[i]);
  }
  printf("\n");
}

// Higher fitness returns a lower number aka penalty function
int fitness_score(int candidate, int target) {
    int fitness_score = abs(target - candidate);
    return fitness_score;
}


void swap(unsigned char* a, unsigned char* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partitionOnFitness(unsigned char *arr, int leftMost, int rightMost, int target) {
    unsigned char pivot = *(arr + rightMost);
    int fitness_pivot = fitness_score(pivot, target);
    int storeIndex = leftMost - 1;

    for (int i = leftMost; i < rightMost; i++) {
        if (fitness_score(arr[i], target) < fitness_pivot) {
            storeIndex++;
            swap(&arr[storeIndex], &arr[i]);
        }
    }
    swap(&arr[storeIndex + 1], &arr[rightMost]);
    return storeIndex + 1;
}

void quickSortFitness(unsigned char *arr, int start, int end, int target) {
    if (start < end) {

        int partition_index = partitionOnFitness(arr, start, end, target);

        // recursion left of pivot
        quickSortFitness(arr, start, partition_index -1, target);

        // recursion right of pivot
        quickSortFitness(arr, partition_index + 1, end, target);
    }
}
