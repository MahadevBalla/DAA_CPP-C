#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants
#define DATA_FILE "random_nos.txt"
#define TIME_FILE "times.csv"
#define NUM_COUNT 100000

// Function to generate and store random numbers
void generateRandomNumbers()
{
    FILE *outFile = fopen(DATA_FILE, "w");
    if (outFile == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < NUM_COUNT; i++)
    {
        fprintf(outFile, "%d ", rand() % 1000000); // Generate random numbers between 0 and 999999
    }
    fclose(outFile);
}

// Function to read numbers from file
void readNumbers(int *numbers, int count)
{
    FILE *inFile = fopen(DATA_FILE, "r");
    if (inFile == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fscanf(inFile, "%d", &numbers[i]); // Read numbers into an array
    }
    fclose(inFile);
}

// Insertion Sort Algorithm
void insertionSort(int *arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Selection Sort Algorithm
void selectionSort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        // Swap the minimum element with the current element
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

// Function to generate best case for Insertion Sort (already sorted array)
void generateBestCaseInsertion(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }
}

// Function to generate worst case for Insertion Sort (reverse sorted array)
void generateWorstCaseInsertion(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = n - i;
    }
}

// Function to generate worst case for Selection Sort (smallest element always at the end)
void generateWorstCaseSelection(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = n - i;
    }
}

// Function to measure sorting time for best and worst cases
void measureSortingTime()
{
    FILE *timeFile = fopen(TIME_FILE, "w");
    if (timeFile == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(timeFile, "Block Size,Insertion Sort Best Case (ms),Insertion Sort Worst Case (ms),Selection Sort Best Case (ms),Selection Sort Worst Case (ms)\n");

    int *numbers = (int *)malloc(NUM_COUNT * sizeof(int));
    if (numbers == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    readNumbers(numbers, NUM_COUNT); // Read numbers from the file

    for (int blockSize = 100; blockSize <= NUM_COUNT; blockSize += 100)
    {
        // Create temporary arrays for sorting
        int *tempInsertionBest = (int *)malloc(blockSize * sizeof(int));
        int *tempInsertionWorst = (int *)malloc(blockSize * sizeof(int));
        int *tempSelectionBest = (int *)malloc(blockSize * sizeof(int));
        int *tempSelectionWorst = (int *)malloc(blockSize * sizeof(int));
        if (tempInsertionBest == NULL || tempInsertionWorst == NULL || tempSelectionBest == NULL || tempSelectionWorst == NULL)
        {
            printf("Memory allocation failed!\n");
            return;
        }

        // Generate best and worst cases for Insertion Sort
        generateBestCaseInsertion(tempInsertionBest, blockSize);
        generateWorstCaseInsertion(tempInsertionWorst, blockSize);

        // Generate best and worst cases for Selection Sort
        generateBestCaseInsertion(tempSelectionBest, blockSize); // Best case for Selection Sort is the same as Insertion Sort
        generateWorstCaseSelection(tempSelectionWorst, blockSize);

        // Measure Insertion Sort time for best case
        clock_t start = clock();
        insertionSort(tempInsertionBest, blockSize);
        clock_t stop = clock();
        double insertionBestTime = (double)(stop - start) * 1000 / CLOCKS_PER_SEC; // Convert to milliseconds

        // Measure Insertion Sort time for worst case
        start = clock();
        insertionSort(tempInsertionWorst, blockSize);
        stop = clock();
        double insertionWorstTime = (double)(stop - start) * 1000 / CLOCKS_PER_SEC; // Convert to milliseconds

        // Measure Selection Sort time for best case
        start = clock();
        selectionSort(tempSelectionBest, blockSize);
        stop = clock();
        double selectionBestTime = (double)(stop - start) * 1000 / CLOCKS_PER_SEC; // Convert to milliseconds

        // Measure Selection Sort time for worst case
        start = clock();
        selectionSort(tempSelectionWorst, blockSize);
        stop = clock();
        double selectionWorstTime = (double)(stop - start) * 1000 / CLOCKS_PER_SEC; // Convert to milliseconds

        // Save results to the CSV file
        fprintf(timeFile, "%d,%.2f,%.2f,%.2f,%.2f\n", blockSize, insertionBestTime, insertionWorstTime, selectionBestTime, selectionWorstTime);
        printf("Block Size: %d - Insertion Best: %.2f ms, Insertion Worst: %.2f ms, Selection Best: %.2f ms, Selection Worst: %.2f ms\n", blockSize, insertionBestTime, insertionWorstTime, selectionBestTime, selectionWorstTime);

        // Free temporary arrays
        free(tempInsertionBest);
        free(tempInsertionWorst);
        free(tempSelectionBest);
        free(tempSelectionWorst);
    }

    fclose(timeFile);
    free(numbers);
}

int main()
{
    // Generate random numbers and save them to a file
    generateRandomNumbers();

    // Measure sorting time for Insertion and Selection Sort
    measureSortingTime();

    printf("Sorting times stored in %s\n", TIME_FILE);
    return 0;
}