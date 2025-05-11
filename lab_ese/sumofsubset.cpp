/*
    Sum of Subsets Problem in C++

    Purpose:
    - Find all subsets of a given array that sum up to a target value (m).
    - Uses recursion and backtracking.

    Time Complexity:
    - O(2^n) in worst case, as it potentially explores all possible subsets.

    How it works:
    1. Recursively include or exclude each element of the array.
    2. When the current sum equals target sum, print the subset.
    3. Use pruning techniques to reduce unnecessary recursive calls:
       - Skip if current sum + current element exceeds target.
       - Skip if current sum + all remaining elements is less than target.
*/

#include <iostream>
#include <vector>
#define MAX 100
using namespace std;

void sumofsubset(vector<int> &arr, int m, int i, int sum, int totalsum, vector<int> list) {
	if(sum == m) {
		for(int i = 0; i < list.size(); i++)
			cout << list[i] << " ";
		cout << endl;
	}
	if(i == arr.size()) return;
	if(sum + arr[i] > m) return;
	if(sum + totalsum < m) return;
	
	// Include arr[i]
	list.push_back(arr[i]);
	sumofsubset(arr, m, i+1, sum+arr[i], totalsum-arr[i],list);
	
	// Exclude arr[i]
	list.pop_back();
	sumofsubset(arr, m, i+1, sum, totalsum-arr[i],list);
}

int main() {
	int n, m;
    cout << "Enter the number of elements (max " << MAX << "): ";
    cin >> n;

    if (n <= 0 || n > MAX) {
        cout << "Invalid number of elements. Must be between 1 and 20.\n";
        return 1;
    }

    vector<int> arr(n);
    cout << "Enter " << n << " elements (space separated): ";
    for (int i = 0; i < n; ++i) cin >> arr[i];

    cout << "Enter the target sum: ";
    cin >> m;

    int totalsum = 0;
    for (auto num: arr) totalsum += num;

    cout << "Subsets with sum " << m << ":\n";
    vector<int> subset;
    sumofsubset(arr, m, 0, 0, totalsum, subset);

    return 0;
}

/*
Enter the number of elements (max 100): 6
Enter 6 elements (space separated): 5 10 12 13 15 18
Enter the target sum: 30

Subsets with sum 30:
5 10 15
5 12 13
12 18
*/