#include <iostream>
#include <vector>
using namespace std;
#define MAX 100

void insertion_sort(vector<int> &arr) {
	int n = arr.size();
	for(int i = 1; i < n; i++) {
		int key = arr[i];
		int j = i-1;
		while(j >= 0 && arr[j] > key) {
			arr[j+1] = arr[j];
			j--;
		}
		arr[j+1]=key;
	}
}

void selection_sort(vector<int> &arr) {
	int n = arr.size();
	for (int i = 0; i < n - 1; i++)
	{
		int min = i;
		for(int j = i+1; j < n; j++) {
			if(arr[j] < arr[min])
				min = j;
		}
		swap(arr[i], arr[min]);
	}
}

int partition(vector<int> &arr, int low, int high) {
	int key = arr[high];
	int i = low-1;
	for(int j = low; j < high; j++) {
		if(arr[j] < key) {
			i++;
			swap(arr[j], arr[i]);
		}
	}
	swap(arr[i+1], arr[high]);
	return i+1;
}

void quick_sort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

void merge(vector<int>& arr, int s, int m, int e) {
	int n1 = m - s + 1;
	int n2 = e - m;

	vector<int> L(n1), R(n2);

	for(int i = 0; i < n1; i++)
		L[i] = arr[s + i];
	for(int i = 0; i < n2; i++)
		R[i] = arr[m + 1 + i];

	int i = 0, j = 0, k = s;

	while(i < n1 && j < n2) {
		if(L[i] < R[j]) {
			arr[k++] = L[i++];
		} else {
			arr[k++] = R[j++];
		}
	}
	while(i < n1) {
		arr[k++] = L[i++];
	}
	while(j < n2) {
		arr[k++] = R[j++];
	}
}

void merge_sort(vector<int>& arr, int s, int e) {
	if(s < e) {
		int mid = s + (e - s) / 2;
		merge_sort(arr, s, mid);
		merge_sort(arr, mid+1, e);
		merge(arr, s, mid, e);
	}
}

void print(vector<int> &arr){
	for(auto &num: arr)
		cout << num << " ";
	cout << endl;
}

int main() {
	int n;
	do {
		cout << "Enter the number of elements: ";
		cin >> n;
		if(n<=0 || n>MAX)
			cout << "Enter a valid input!" << endl;
	} while (n<=0 || n>MAX);

	vector<int> arr(n);
	cout << "Enter the integers: ";
	for (int i=0; i<n; i++) {
		cin >> arr[i];
	}

	printf("\nBefore sorting: ");
	print(arr);

	// insertion_sort(arr);
	// selection_sort(arr);
	// quick_sort(arr, 0, n-1);
	merge_sort(arr, 0, n-1);

	printf("After sorting: ");
	print(arr);
	return 0;
}