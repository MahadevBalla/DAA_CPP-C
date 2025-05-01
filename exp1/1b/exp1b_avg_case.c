#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_FILE "random_numbers.txt"
#define TIME_FILE "sorting_times.csv"
#define NUM_COUNT 100000

void generateRandomNumbers() {
  FILE *outFile = fopen(DATA_FILE, "w");
  if (outFile == NULL) {
    printf("Error opening file!\n");
    return;
  }
  srand(time(0));
  for (int i = 0; i < NUM_COUNT; i++) {
    fprintf(outFile, "%d ", rand() % 1000000);
  }
  fclose(outFile);
}

void readNumbers(int *numbers, int count) {
  FILE *inFile = fopen(DATA_FILE, "r");
  if (inFile == NULL) {
    printf("Error opening file!\n");
    return;
  }
  for (int i = 0; i < count; i++) {
    fscanf(inFile, "%d", &numbers[i]);
  }
  fclose(inFile);
}

void insertionSort(int *arr, int n) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void selectionSort(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    int minIndex = i;
    for (int j = i + 1; j < n; j++) {
      if (arr[j] < arr[minIndex]) {
        minIndex = j;
      }
    }
    int temp = arr[i];
    arr[i] = arr[minIndex];
    arr[minIndex] = temp;
  }
}

void measureSortingTime() {
  FILE *timeFile = fopen(TIME_FILE, "w");
  if (timeFile == NULL) {
    printf("Error opening file!\n");
    return;
  }
  fprintf(timeFile,
          "Block Size,Insertion Sort Time (ms),Selection Sort Time (ms)\n");

  int *numbers = (int *)malloc(NUM_COUNT * sizeof(int));
  if (numbers == NULL) {
    printf("Memory allocation failed!\n");
    return;
  }
  readNumbers(numbers, NUM_COUNT);

  for (int blockSize = 100; blockSize <= NUM_COUNT; blockSize += 100) {
    int *tempInsertion = (int *)malloc(blockSize * sizeof(int));
    int *tempSelection = (int *)malloc(blockSize * sizeof(int));
    if (tempInsertion == NULL || tempSelection == NULL) {
      printf("Memory allocation failed!\n");
      return;
    }

    for (int i = 0; i < blockSize; i++) {
      tempInsertion[i] = numbers[i];
      tempSelection[i] = numbers[i];
    }

    clock_t start = clock();
    insertionSort(tempInsertion, blockSize);
    clock_t stop = clock();
    double insertionTime = (double)(stop - start) * 1000 /
                           CLOCKS_PER_SEC; // Convert to milliseconds

    start = clock();
    selectionSort(tempSelection, blockSize);
    stop = clock();
    double selectionTime = (double)(stop - start) * 1000 / CLOCKS_PER_SEC;

    fprintf(timeFile, "%d,%.2f,%.2f\n", blockSize, insertionTime,
            selectionTime);
    printf("Block Size: %d - Insertion: %.2f ms, Selection: %.2f ms\n",
           blockSize, insertionTime, selectionTime);

    free(tempInsertion);
    free(tempSelection);
  }

  fclose(timeFile);
  free(numbers);
}

int main() {
  generateRandomNumbers();

  measureSortingTime();

  printf("Sorting times stored in %s\n", TIME_FILE);
  return 0;
}