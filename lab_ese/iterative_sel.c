
#include <stdio.h>

// show the array
void show(int x[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", x[i]);
    printf("\n");
}

// sort the array
void ssort(int x[], int n)
{
    int i, j, min, tmp;
    
    printf("\nStarting sort...\n");
    
    for (i = 0; i < n - 1; i++) {
        min = i;  // assume first is min
        
        // find actual min
        for (j = i + 1; j < n; j++) {
            if (x[j] < x[min])
                min = j;
        }
        
        // print iteration info
        printf("Iteration %d: ", i+1);
        
        // swap em
        if (min != i) {
            printf("Swap %d (pos %d) <-> %d (pos %d)\n", x[i], i, x[min], min);
            tmp = x[i];
            x[i] = x[min];
            x[min] = tmp;
        } else {
            printf("No swap needed (min already at pos %d)\n", i);
        }
        
        // show current state
        printf("Current array: ");
        show(x, n);
        printf("\n");
    }
}

int main()
{
    int nums[100]; // max 100 numbers
    int size, i;
    
    // get input size
    printf("Enter size: ");
    scanf("%d", &size);
    
    if (size <= 0 || size > 100) {
        printf("Error: size must be 1-100\n");
        return 1;
    }
    
    // get numbers
    printf("Enter %d numbers:\n", size);
    for (i = 0; i < size; i++) {
        scanf("%d", &nums[i]);
    }
    
    printf("\nOriginal array: ");
    show(nums, size);
    
    ssort(nums, size);
    
    printf("\nFinal sorted array: ");
    show(nums, size);
    
    return 0;
}