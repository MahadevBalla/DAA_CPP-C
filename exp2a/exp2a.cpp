#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

#define NUM_COUNT 100000
#define OUTPUT_FILE "random_numbers.txt"
#define TIME_RESULT_FILE "sorting_times.csv"

using namespace std;

void generate_random_numbers()
{
    ofstream file(OUTPUT_FILE);
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    for (int i = 0; i < NUM_COUNT; i++) {
        file << rand() % 1000000 << "\n";
    }
    file.close();
}

void read_numbers(vector<int>& arr, int size) {
    ifstream file(OUTPUT_FILE);
    if (!file) {
        cerr << "Error opening file for reading!" << endl;
        return;
    }
    arr.resize(size);
    for (int i = 0; i < size; i++) {
        file >> arr[i];
    }
    file.close();
}

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right)
        temp.push_back((arr[i] <= arr[j]) ? arr[i++] : arr[j++]);

    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int i = left; i <= right; i++)
        arr[i] = temp[i - left];
}

void merge_sort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    for (int j = low + 1; j <= high; j++) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[low], arr[i - 1]);
    return i - 1;
}

void quick_sort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

void perform_experiment() {
    ofstream file(TIME_RESULT_FILE);
    if (!file) {
        cerr << "Error opening file for writing results!" << endl;
        return;
    }
    file << "Block Size,QuickSort Random (ms),MergeSort Random (ms),QuickSort Best (ms),MergeSort Best (ms),QuickSort Worst (ms),MergeSort Worst (ms)\n";

    for (int block_size = 100; block_size <= NUM_COUNT; block_size += 100) {
        vector<int> arr1, arr2;
        read_numbers(arr1, block_size);
        arr2 = arr1;

        auto start = high_resolution_clock::now();
        quick_sort(arr1, 0, block_size - 1);
        auto end = high_resolution_clock::now();
        double quicksort_random = duration<double, milli>(end - start).count();

        start = high_resolution_clock::now();
        merge_sort(arr2, 0, block_size - 1);
        end = high_resolution_clock::now();
        double mergesort_random = duration<double, milli>(end - start).count();

        sort(arr1.begin(), arr1.end()); // Best case sorted input
        start = high_resolution_clock::now();
        quick_sort(arr1, 0, block_size - 1);
        end = high_resolution_clock::now();
        double quicksort_best = duration<double, milli>(end - start).count();

        start = high_resolution_clock::now();
        merge_sort(arr2, 0, block_size - 1);
        end = high_resolution_clock::now();
        double mergesort_best = duration<double, milli>(end - start).count();

        sort(arr1.rbegin(), arr1.rend()); // Worst case reversed input
        start = high_resolution_clock::now();
        quick_sort(arr1, 0, block_size - 1);
        end = high_resolution_clock::now();
        double quicksort_worst = duration<double, milli>(end - start).count();

        start = high_resolution_clock::now();
        merge_sort(arr2, 0, block_size - 1);
        end = high_resolution_clock::now();
        double mergesort_worst = duration<double, milli>(end - start).count();

        file << block_size << "," << quicksort_random << "," << mergesort_random << "," << quicksort_best << "," << mergesort_best << "," << quicksort_worst << "," << mergesort_worst << "\n";

        cout << "Block Size: " << block_size << " | QuickSort Random: " << quicksort_random << " ms | MergeSort Random: " << mergesort_random
            << " ms | QuickSort Best: " << quicksort_best << " ms | MergeSort Best: " << mergesort_best
            << " ms | QuickSort Worst: " << quicksort_worst << " ms | MergeSort Worst: " << mergesort_worst << " ms" << endl;
    }
    file.close();
}

int main() {
    srand(time(nullptr));
    generate_random_numbers();
    perform_experiment();
    cout << "Experiment complete. Results saved in " << TIME_RESULT_FILE << "." << endl;
    return 0;
}
