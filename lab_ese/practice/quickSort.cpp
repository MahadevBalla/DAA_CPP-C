#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int> &arr, int s, int e) {
  int key = arr[s];
  int i = s;
  for (int j = s+1; j<=e; j++) {
    if (arr[j] < key) {
      i++;
      swap(arr[j], arr[i]);
    }
  }
  swap(arr[s], arr[i]);
  return i;
}

void quickSort(vector<int> &arr, int s, int e) {
  if (s < e) {
    int pi = partition(arr, s, e);
    quickSort(arr, s, pi - 1);
    quickSort(arr, pi + 1, e);
  }
}

void printArr(vector<int> &arr) {
  cout << "Elements of the array are: ";
  for (auto &it : arr) {
    cout << it << " ";
  }
  cout << endl;
}

int main() {
  int n;
  cout << "Enter the no. of elements: ";
  cin >> n;
  vector<int> arr(n);
  cout << "Enter the elements: ";
  for (int i = 0; i < n; i++)
    cin >> arr[i];

  quickSort(arr, 0, n - 1);
  printArr(arr);
  return 0;
}