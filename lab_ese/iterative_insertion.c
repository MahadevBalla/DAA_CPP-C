#include <stdio.h>

// prints the array
void show(int a[], int len) {
  for (int i = 0; i < len; i++)
    printf("%d ", a[i]);
  printf("\n");
}

// sorts array using insertion method
void sort(int a[], int len) {
  int i, j, tmp;

  printf("\nStarting insertion sort...\n");

  for (i = 1; i < len; i++) {
    tmp = a[i];
    j = i - 1;

    printf("\nIteration %d: Insert %d into sorted portion\n", i, tmp);
    printf("Current array: ");
    show(a, len);

    // shift elements to right if greater than tmp
    printf("Shifting: ");
    int shifted = 0;

    while (j >= 0 && a[j] > tmp) {
      printf("%d moves right, ", a[j]);
      a[j + 1] = a[j];
      j--;
      shifted = 1;
    }

    if (!shifted) {
      printf("No shifts needed");
    }

    a[j + 1] = tmp;
    printf("\nPlaced %d at position %d\n", tmp, j + 1);

    printf("After iteration %d: ", i);
    show(a, len);
  }
}

int main() {
  int nums[100]; // max 100 elements
  int size = 0;
  int i;

  printf("Enter number of elements (max 100): ");
  scanf("%d", &size);

  if (size <= 0 || size > 100) {
    printf("Invalid size! Must be between 1-100\n");
    return 1;
  }

  printf("Enter %d numbers:\n", size);
  for (i = 0; i < size; i++) {
    scanf("%d", &nums[i]);
  }

  printf("\nOriginal array: ");
  show(nums, size);

  // do the sort
  sort(nums, size);

  printf("\nFinal sorted array: ");
  show(nums, size);

  return 0;
}