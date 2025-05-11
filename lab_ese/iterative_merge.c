#include <stdio.h>
#include <stdlib.h>

// print an array
void show(int arr[], int size, char *msg) {
  printf("%s", msg);
  for (int i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

// merge two sorted parts
void mix(int arr[], int left, int mid, int right, int depth) {
  int i, j, k;
  int spaces[10] = {0};

  // indent based on recursion depth
  for (i = 0; i < depth; i++)
    spaces[i] = 1;

  for (i = 0; i < depth; i++)
    printf("  ");
  printf("Merging: ");
  for (i = left; i <= right; i++)
    printf("%d ", arr[i]);
  printf("\n");

  int n1 = mid - left + 1;
  int n2 = right - mid;

  // temp arrays
  int *L = malloc(n1 * sizeof(int));
  int *R = malloc(n2 * sizeof(int));

  // copy to temp arrays
  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  for (i = 0; i < depth; i++)
    printf("  ");
  printf("Left half: ");
  for (i = 0; i < n1; i++)
    printf("%d ", L[i]);
  printf("\n");

  for (i = 0; i < depth; i++)
    printf("  ");
  printf("Right half: ");
  for (j = 0; j < n2; j++)
    printf("%d ", R[j]);
  printf("\n");

  // merge back
  i = 0;
  j = 0;
  k = left;

  for (i = 0; i < depth; i++)
    printf("  ");
  printf("Comparing and merging...\n");

  i = 0;
  j = 0;

  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      for (int x = 0; x < depth; x++)
        printf("  ");
      printf("Take %d from left\n", L[i]);
      arr[k] = L[i];
      i++;
    } else {
      for (int x = 0; x < depth; x++)
        printf("  ");
      printf("Take %d from right\n", R[j]);
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  // copy remaining elements
  while (i < n1) {
    for (int x = 0; x < depth; x++)
      printf("  ");
    printf("Take remaining %d from left\n", L[i]);
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    for (int x = 0; x < depth; x++)
      printf("  ");
    printf("Take remaining %d from right\n", R[j]);
    arr[k] = R[j];
    j++;
    k++;
  }

  for (i = 0; i < depth; i++)
    printf("  ");
  printf("After merge: ");
  for (i = left; i <= right; i++)
    printf("%d ", arr[i]);
  printf("\n\n");

  free(L);
  free(R);
}

// recursive merge sort
void msort(int arr[], int left, int right, int depth) {
  if (left < right) {
    int i;

    // indent based on recursion depth
    for (i = 0; i < depth; i++)
      printf("  ");

    printf("Split [%d...%d]: ", left, right);
    for (i = left; i <= right; i++)
      printf("%d ", arr[i]);
    printf("\n");

    int mid = left + (right - left) / 2;

    // sort first and second halves
    msort(arr, left, mid, depth + 1);
    msort(arr, mid + 1, right, depth + 1);

    // merge the sorted halves
    mix(arr, left, mid, right, depth);
  } else {
    for (int i = 0; i < depth; i++)
      printf("  ");
    printf("Single element [%d]: %d (already sorted)\n\n", left, arr[left]);
  }
}

int main() {
  int nums[100]; // max 100 elements
  int size;

  printf("Enter the number of elements (max 100): ");
  scanf("%d", &size);

  if (size <= 0 || size > 100) {
    printf("Invalid size! Must be between 1-100\n");
    return 1;
  }

  printf("Enter %d numbers:\n", size);
  for (int i = 0; i < size; i++) {
    scanf("%d", &nums[i]);
  }

  printf("\nOriginal array: ");
  for (int i = 0; i < size; i++)
    printf("%d ", nums[i]);
  printf("\n\n");

  printf("MERGE SORT STEPS:\n");
  printf("----------------\n\n");

  // start merge sort
  msort(nums, 0, size - 1, 0);

  printf("Final sorted array: ");
  for (int i = 0; i < size; i++)
    printf("%d ", nums[i]);
  printf("\n");

  return 0;
}
