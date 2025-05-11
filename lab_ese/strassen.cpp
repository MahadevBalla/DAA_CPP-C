/*
* Strassen's Matrix Multiplication Algorithm
* 
* This algorithm multiplies two square matrices of size n×n where n is a power of 2.
* It works by recursively breaking down the multiplication into 7 smaller multiplications.
* (instead of 8 as in the naive recursive approach), achieving better theoretical complexity.
* 
* How it works:
* 1. Divide both input matrices into 4 submatrices of size n/2 × n/2
* 2. Compute 7 products P1 through P7 using these submatrices
* 3. Combine these products to form the 4 quadrants of the result matrix
* 
* Time Complexity: O(n^log_2(7)) ≈ O(n^2.81), better than naive O(n^3)
* Space Complexity: O(n^2) for storing matrices
* 
* Note: For small matrices, the overhead of recursion may make this slower than
* the standard algorithm in practice. Typically used for very large matrices.
*
* Simplified Strassen's Matrix Multiplication Algorithm
* 
* Multiplies two matrices using Strassen's divide-and-conquer approach
* which has O(n^2.81) time complexity instead of the standard O(n^3).
* 
* This works best for large matrices where n is a power of 2.
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Matrix addition/subtraction
vector<vector<int>> add(const vector<vector<int>> &mat1, const vector<vector<int>> &mat2, int sign = 1) {
    int n = mat1.size();
    vector<vector<int>> result(n, vector<int>(n));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = mat1[i][j] + sign * mat2[i][j];
    return result;
}

// Strassen's recursive multiplication
vector<vector<int>> strassen(const vector<vector<int>> &mat1, const vector<vector<int>> &mat2) {
    int n = mat1.size();
    vector<vector<int>> res(n, vector<int>(n));

    // Base case
    if(n == 1) {
        res[0][0] = mat1[0][0] * mat2[0][0];
        return res;
    }

    int half = n / 2;

    // Create submatrices
    vector<vector<int>> a(half, vector<int>(half));
    vector<vector<int>> b(half, vector<int>(half));
    vector<vector<int>> c(half, vector<int>(half));
    vector<vector<int>> d(half, vector<int>(half));
    vector<vector<int>> e(half, vector<int>(half));
    vector<vector<int>> f(half, vector<int>(half));
    vector<vector<int>> g(half, vector<int>(half));
    vector<vector<int>> h(half, vector<int>(half));

    // Create submatrices
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            a[i][j] = mat1[i][j];
            b[i][j] = mat1[i][j + half];
            c[i][j] = mat1[i + half][j];
            d[i][j] = mat1[i + half][j + half];

            e[i][j] = mat2[i][j];
            f[i][j] = mat2[i][j + half];
            g[i][j] = mat2[i + half][j];
            h[i][j] = mat2[i + half][j + half];
        }
    }

    vector<vector<int>> P1 = strassen(a, add(f, h, -1));    // P1 = a * (f - h)
    vector<vector<int>> P2 = strassen(add(a, b), h);    // P2 = (a + b) * h
    vector<vector<int>> P3 = strassen(add(c, d), e);    // P3 = (c + d) * e
    vector<vector<int>> P4 = strassen(d, add(g, e, -1));    // P4 = d * (g - e)
    vector<vector<int>> P5 = strassen(add(a, d), add(e, h));    // P5 = (a + d) * (e + h)
    vector<vector<int>> P6 = strassen(add(b, d, -1), add(g, h));    // P6 = (b - d) * (g + h)
    vector<vector<int>> P7 = strassen(add(a, c, -1), add(e, f));    // P7 = (a - c) * (e + f)

    // Final quadrants
    vector<vector<int>> r = add(add(P5, P4), add(P6, P2, -1));
    vector<vector<int>> s = add(P1, P2);
    vector<vector<int>> t = add(P3, P4);
    vector<vector<int>> u = add(add(P1, P5), add(P3, P7), -1);

    // Combine quadrants into result
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            res[i][j] = r[i][j];
            res[i][j + half] = s[i][j];
            res[i + half][j] = t[i][j];
            res[i + half][j + half] = u[i][j];
        }
    }

    return res;
}

// Function to calculate the nearest power of 2 greater than or equal to n
int nextPowerOfTwo(int n) {
    return pow(2, ceil(log2(n)));
}

// Function to pad matrix to the next power of 2
vector<vector<int>> padMatrix(const vector<vector<int>> &mat, int newSize) {
    int oldSize = mat.size();
    vector<vector<int>> padded(newSize, vector<int>(newSize, 0));

    for (int i = 0; i < oldSize; i++) {
        for (int j = 0; j < oldSize; j++) {
            padded[i][j] = mat[i][j];
        }
    }
    return padded;
}

int main() {
    int n;
    cout << "Enter the size of square matrix (power of 2): ";
    cin >> n;

    // Find the next power of 2
    int newSize = nextPowerOfTwo(n);

    vector<vector<int>> A(n, vector<int>(n)), B(n, vector<int>(n));

    cout << "Enter elements of first matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    cout << "Enter elements of second matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> B[i][j];
        }
    }
    
    // Pad the matrices to the next power of 2
    vector<vector<int>> paddedA = padMatrix(A, newSize);
    vector<vector<int>> paddedB = padMatrix(B, newSize);

    // Perform Strassen's multiplication on padded matrices
    vector<vector<int>> paddedResult = strassen(paddedA, paddedB);

    // Extract the result (remove the padding)
    vector<vector<int>> result(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = paddedResult[i][j];
        }
    }

    cout << "Result of matrix multiplication:\n";
    for (const auto &row : result) {
        for (auto val : row)
            cout << val << " ";
        cout << endl;
    }

    return 0;
}

/* Example input/output:
Enter the size of square matrix (power of 2): 2
Enter elements of first matrix:
7 8
2 9
Enter elements of second matrix:
14 5
5 18
Result of matrix multiplication:
138 179
73 172
*/