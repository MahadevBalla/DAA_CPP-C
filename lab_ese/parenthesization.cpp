/*
 	Matrix Chain Multiplication - Dynamic Programming Implementation
	 	This program finds the most efficient way to multiply a chain of matrices
 		to minimize the total number of scalar multiplications needed.
	
	How it works:
 		1. For a chain of n matrices, we have n-1 matrices with dimensions in array p[0...n]
 		2. Matrix i has dimensions p[i-1] x p[i]
 		3. The algorithm uses dynamic programming to compute the minimum cost of multiplications
 		   by trying all possible ways to parenthesize the chain
 		4. It builds a table dp[i][j] which represents minimum cost for multiplying matrices from i to j
 		5. It also tracks the optimal split points in a separate table for reconstructing the parenthesization
	
	Time Complexity: O(n³) where n is the number of matrices
 	Space Complexity: O(n²)
*/

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void printP(vector<vector<int>> &split, int i, int j, char & ch) {
	if(i + 1 == j) {
		cout << ch++;
	}else {
		cout << "(";
		printP(split, i, split[i][j], ch);
		printP(split, split[i][j], j, ch);
		cout << ")";
	}	

}

int matrix_mult(vector<int> &p) {
	int n = p.size();
	vector<vector<int>> dp(n, vector<int>(n, 0));
	vector<vector<int>> split(n, vector<int>(n, 0));
	
	for(int len = 2; len < n; len++) {
		for(int i = 0; i < n-len; i++) {
			int j = i+len;
			dp[i][j] = INT_MAX;
			for(int k = i+1; k < j; k++) {
				int cost = dp[i][k]+dp[k][j]+p[i]*p[k]*p[j];
				if(cost < dp[i][j]) {
					dp[i][j] = cost;
					split[i][j] = k;
				}
			} 
		}
	}


	char ch = 'A';
	cout << "Optimal Paranthesization: ";
	printP(split, 0, n-1, ch);
	cout << endl;
	return dp[0][n - 1];
}

int main() {
	int n;
    do{
		cout << "Enter number of matrices: ";
    	cin >> n;
		if(n<=0) cout << "Invalid number of matrices.\n";
	} while (n <= 0);

	vector<int> p(n + 1);
    cout << "Enter " << n + 1 << " dimensions (for " << n << " matrices):\n";
    for (int i = 0; i <= n; ++i) {
        cin >> p[i];
    }

    cout << "\nMatrix dimensions:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Matrix " << char('A' + i) << ": " << p[i] << " x " << p[i + 1] << "\n";
    }

    int minCost = matrix_mult(p);
    cout << "Minimum number of scalar multiplications: " << minCost << endl;
	return 0;
}

/*
 Enter number of matrices: 4
Enter 5 dimensions (for 4 matrices):
5 
4  
6
2
7

Matrix dimensions:
Matrix A: 5 x 4
Matrix B: 4 x 6
Matrix C: 6 x 2
Matrix D: 2 x 7
Optimal Parenthesization: ((A(BC))D)
Minimum number of scalar multiplications: 158
*/