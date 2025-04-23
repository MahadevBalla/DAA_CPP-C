#include <stdio.h>
#include <stdlib.h>

int flag = 0;

void printsubset(int subset[], int size) {
  printf("[");
  for (int i = 0; i < size; i++)
    printf("%d ", subset[i]);
  printf("]\n");
}

void subsetsum(int index, int n, int set[], int targetsum, int subset[],
               int subsetsize) {
  if (targetsum == 0) {
    flag = 1;
    printsubset(subset, subsetsize);
    return;
  }

  if (index == n)
    return;

  if (set[index] <= targetsum) {
    subset[subsetsize] = set[index];
    subsetsum(index + 1, n, set, targetsum - set[index], subset,
              subsetsize + 1);
  }
  subsetsum(index + 1, n, set, targetsum, subset, subsetsize);
}

int main() {
  int n, targetsum;

  printf("Enter the number of elements in the set: ");
  scanf("%d", &n);

  int *set = (int *)malloc(n * sizeof(int));

  printf("Enter the elements of the set: ");
  for (int i = 0; i < n; i++) {
    scanf("%d", &set[i]);
  }

  printf("Enter the target sum: ");
  scanf("%d", &targetsum);

  int *subset = (int *)malloc(n * sizeof(int));

  printf("Subsets with sum %d:\n", targetsum);

  subsetsum(0, n, set, targetsum, subset, 0);

  return 0;
}