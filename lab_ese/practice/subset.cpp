#include <iostream>
#include <vector>
using namespace std;

void printSubset(vector<int> &arr, int sum, int m, int i, int totalSum, vector<int> &ans) {
  if (sum == m) {
    for (int j=0; j<ans.size(); j++) {
      cout << ans[j] << " ";
    }
    cout << endl;
  }
  if (i == arr.size())
    return;
  if (sum+arr[i] > m)
    return;
  if (sum+totalSum < m)
    return;

  ans.push_back(arr[i]);
  printSubset(arr, sum + arr[i], m, i + 1, totalSum - arr[i], ans);

  ans.pop_back();
  printSubset(arr, sum, m, i + 1, totalSum - arr[i], ans);
}

int main() {
  int m;
  cout << "Enter the target sum: ";
  cin >> m;

  int n;
  cout << "Enter the no.of elements in the array: ";
  cin >> n;

  vector<int> arr(n);
  int totalSum = 0;
  cout << "Enter the elements of the arr: ";
  for (int i=0; i<n; i++) {
    cin >> arr[i];
    totalSum += arr[i];
  }

  vector<int> ans;
  printSubset(arr, 0, m, 0, totalSum, ans);

  return 0;
}