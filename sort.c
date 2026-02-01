#include <stdio.h>

// function to print array elements
void printArray(unsigned char array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf("%u  ", array[i]);
  }
  printf("\n");
}

void swap(unsigned char* a, unsigned char* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(unsigned char *arr, int leftMost, int rightMost) {
    unsigned char pivot = *(arr + rightMost);
    int storeIndex = leftMost - 1;

    for (int i = leftMost; i < rightMost; i++) {
        if (arr[i] < pivot) {
            storeIndex++;
            swap(&arr[storeIndex], &arr[i]);
        }
    }
    swap(&arr[storeIndex + 1], &arr[rightMost]);
    return storeIndex + 1;
}

void quickSort(unsigned char *arr, int start, int end) {
    if (start < end) {

        int partition_index = partition(arr, start, end);

        // recursion left of pivot
        quickSort(arr, start, partition_index -1);

        // recursion right of pivot
        quickSort(arr, partition_index + 1, end);
    }
}
