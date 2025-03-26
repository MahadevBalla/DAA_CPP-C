#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>

typedef struct {
    double** data;
    int rows;
    int cols;
} Matrix;

// Creates a new matrix with given dimensions
Matrix createMatrix(int rows, int cols) {
    Matrix M;
    M.rows = rows;
    M.cols = cols;
    M.data = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        M.data[i] = (double*)malloc(cols * sizeof(double));
    }
    return M;
}

// Free memory allocated for a matrix
void freeMatrix(Matrix M) {
    for (int i = 0; i < M.rows; i++) {
        free(M.data[i]);
    }
    free(M.data);
}

// Generates a matrix with random values between 0 and 1
Matrix generateRandomMatrix(int rows, int cols) {
    Matrix M = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            M.data[i][j] = (double)rand() / RAND_MAX; // Random value [0,1]
        }
    }
    return M;
}

// Prints matrix with formatted output
void printMatrix(Matrix M) {
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.cols; j++) {
            printf("%8.4f ", M.data[i][j]); // 4 decimal places, 8 width
        }
        printf("\n");
    }
}

// Standard O(n³) matrix multiplication - triple loop for matrix multiplication
Matrix regularMultiply(Matrix A, Matrix B) {
    int r = A.rows;
    int c = B.cols;
    int inner = A.cols;
    Matrix C = createMatrix(r, c);

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            C.data[i][j] = 0.0;
            for (int k = 0; k < inner; k++) {
                C.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return C;
}

// Matrix addition
Matrix addMatrix(Matrix A, Matrix B) {
    int r = A.rows, c = A.cols;
    Matrix C = createMatrix(r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            C.data[i][j] = A.data[i][j] + B.data[i][j];
        }
    }
    return C;
}

// Matrix subtraction
Matrix subMatrix(Matrix A, Matrix B) {
    int r = A.rows, c = A.cols;
    Matrix C = createMatrix(r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            C.data[i][j] = A.data[i][j] - B.data[i][j];
        }
    }
    return C;
}

// Extracts a submatrix from given position
Matrix getSubmatrix(Matrix A, int row, int col, int size) {
    Matrix sub = createMatrix(size, size);
    // Copy elements from original matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            sub.data[i][j] = A.data[row + i][col + j];
        }
    }
    return sub;
}

// Copies submatrix into another matrix at given position
void setSubmatrix(Matrix* C, Matrix sub, int row, int col) {
    int size = sub.rows;
    // Copy elements to target matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C->data[row + i][col + j] = sub.data[i][j];
        }
    }
}

// Pads matrix with zeros to make it square of size n×n
Matrix padMatrix(Matrix A, int n) {
    Matrix B = createMatrix(n, n);
    // Copy original elements, pad with zeros
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B.data[i][j] = (i < A.rows && j < A.cols) ? A.data[i][j] : 0.0;
        }
    }
    return B;
}

// Removes padding to restore original dimensions
Matrix unpadMatrix(Matrix A, int rows, int cols) {
    Matrix B = createMatrix(rows, cols);
    // Copy only the original portion
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            B.data[i][j] = A.data[i][j];
        }
    }
    return B;
}

// Strassen's algorithm for square matrices (recursive)
Matrix strassenMultiplySquare(Matrix A, Matrix B) {
    int n = A.rows;
    Matrix C = createMatrix(n, n);

    // Base case: 1×1 matrix
    if (n == 1) {
        C.data[0][0] = A.data[0][0] * B.data[0][0];
        return C;
    }

    int newSize = n / 2;

    // Divide matrices into 4 submatrices each
    Matrix A11 = getSubmatrix(A, 0, 0, newSize);
    Matrix A12 = getSubmatrix(A, 0, newSize, newSize);
    Matrix A21 = getSubmatrix(A, newSize, 0, newSize);
    Matrix A22 = getSubmatrix(A, newSize, newSize, newSize);

    Matrix B11 = getSubmatrix(B, 0, 0, newSize);
    Matrix B12 = getSubmatrix(B, 0, newSize, newSize);
    Matrix B21 = getSubmatrix(B, newSize, 0, newSize);
    Matrix B22 = getSubmatrix(B, newSize, newSize, newSize);

    // Compute the 7 products recursively
    Matrix M1 = strassenMultiplySquare(addMatrix(A11, A22), addMatrix(B11, B22));
    Matrix M2 = strassenMultiplySquare(addMatrix(A21, A22), B11);
    Matrix M3 = strassenMultiplySquare(A11, subMatrix(B12, B22));
    Matrix M4 = strassenMultiplySquare(A22, subMatrix(B21, B11));
    Matrix M5 = strassenMultiplySquare(addMatrix(A11, A12), B22);
    Matrix M6 = strassenMultiplySquare(subMatrix(A21, A11), addMatrix(B11, B12));
    Matrix M7 = strassenMultiplySquare(subMatrix(A12, A22), addMatrix(B21, B22));

    // Compute result submatrices using Strassen's formulas
    Matrix C11 = addMatrix(subMatrix(addMatrix(M1, M4), M5), M7);
    Matrix C12 = addMatrix(M3, M5);
    Matrix C21 = addMatrix(M2, M4);
    Matrix C22 = addMatrix(subMatrix(addMatrix(M1, M3), M2), M6);

    // Combine submatrices into final result
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C.data[i][j] = C11.data[i][j];
            C.data[i][j + newSize] = C12.data[i][j];
            C.data[i + newSize][j] = C21.data[i][j];
            C.data[i + newSize][j + newSize] = C22.data[i][j];
        }
    }

    // Free all temporary matrices
    freeMatrix(A11); freeMatrix(A12); freeMatrix(A21); freeMatrix(A22);
    freeMatrix(B11); freeMatrix(B12); freeMatrix(B21); freeMatrix(B22);
    freeMatrix(M1); freeMatrix(M2); freeMatrix(M3); freeMatrix(M4);
    freeMatrix(M5); freeMatrix(M6); freeMatrix(M7);
    freeMatrix(C11); freeMatrix(C12); freeMatrix(C21); freeMatrix(C22);

    return C;
}

// Strassen's algorithm with padding for non-square matrices
Matrix strassenMultiply(Matrix A, Matrix B) {
    int r1 = A.rows;
    int c1 = A.cols;
    int r2 = B.rows;
    int c2 = B.cols;

    // Find smallest power of 2 that can contain all dimensions
    int n = r1;
    if (c1 > n) n = c1;
    if (r2 > n) n = r2;
    if (c2 > n) n = c2;

    int mSize = 1;
    while (mSize < n) mSize *= 2;

    // Pad matrices to make them square with power-of-2 dimensions
    Matrix A_padded = padMatrix(A, mSize);
    Matrix B_padded = padMatrix(B, mSize);
    Matrix C_padded = strassenMultiplySquare(A_padded, B_padded);
    Matrix C = unpadMatrix(C_padded, r1, c2);

    // Free temporary padded matrices
    freeMatrix(A_padded);
    freeMatrix(B_padded);
    freeMatrix(C_padded);

    return C;
}

// Dynamic programming solution for matrix chain ordering
void matrixChainOrder(int* p, int n, long long** m, int** s) {
    // Initialize diagonal (single matrix costs 0)
    for (int i = 1; i <= n; i++) {
        m[i][i] = 0;
    }

    // Fill DP tables for chain lengths from 2 to n
    for (int l = 2; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            m[i][j] = LLONG_MAX;
            // Try all possible split points
            for (int k = i; k < j; k++) {
                long long q = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k; // Store optimal split point
                }
            }
        }
    }
}

// Recursively prints optimal parenthesization
void printOptimalParens(int** s, int i, int j) {
    if (i == j) {
        printf("M%d", i); // Base case: single matrix
    }
    else {
        printf("(");
        printOptimalParens(s, i, s[i][j]); // Left subexpression        
        printf(" X ");
        printOptimalParens(s, s[i][j] + 1, j); // Right subexpression
        printf(")");
    }
}

// Multiplies matrix chain using optimal parenthesization
Matrix multiplyChainOptimal(Matrix* matrices, int** s, int i, int j, int useStrassen) {
    if (i == j) {
        return matrices[i - 1]; // Base case: return single matrix
    }
    int k = s[i][j]; // Optimal split point

    // Recursively multiply left and right parts
    Matrix A = multiplyChainOptimal(matrices, s, i, k, useStrassen);
    Matrix B = multiplyChainOptimal(matrices, s, k + 1, j, useStrassen);
    Matrix result;

    // Use specified multiplication algorithm
    if (useStrassen) {
        result = strassenMultiply(A, B);
    }
    else {
        result = regularMultiply(A, B);
    }

    // Free intermediate matrices if they were created
    if (i != k) freeMatrix(A);
    if (k + 1 != j) freeMatrix(B);
    return result;
}

// Multiplies matrix chain in trivial left-to-right order
Matrix multiplyChainTrivial(Matrix* matrices, int n, int useStrassen) {
    Matrix result = matrices[0];
    for (int i = 1; i < n; i++) {
        Matrix temp;
        // Use specified multiplication algorithm
        if (useStrassen) {
            temp = strassenMultiply(result, matrices[i]);
        }
        else {
            temp = regularMultiply(result, matrices[i]);
        }

        // Free previous result if it wasn't the first matrix
        if (i > 1) freeMatrix(result);
        result = temp;
    }
    return result;
}

int main() {
    srand(time(0)); // Seed random number generator
    int n = 10; // Number of matrices in chain
    int p[n + 1]; // Array of matrix dimensions

    int possible[] = { 8, 16, 32, 64 }; // Possible matrix dimensions (powers of 2 for Strassen)
    for (int i = 0; i < n + 1; i++) {
        p[i] = possible[rand() % 4]; // Randomly select dimensions
    }

    printf("Matrix dimensions array p: ");
    for (int i = 0; i < n + 1; i++) {
        printf("%d ", p[i]);
    }
    printf("\n");

    // Generate random matrices with specified dimensions
    Matrix* matrices = (Matrix*)malloc(n * sizeof(Matrix));
    for (int i = 0; i < n; i++) {
        int rows = p[i];
        int cols = p[i + 1];
        matrices[i] = generateRandomMatrix(rows, cols);
    }

    // Allocate m and s matrices for matrix chain ordering
    long long** m = (long long**)malloc((n + 1) * sizeof(long long*));
    int** s = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        m[i] = (long long*)malloc((n + 1) * sizeof(long long));
        s[i] = (int*)malloc((n + 1) * sizeof(int));
    }

    matrixChainOrder(p, n, m, s); // Compute optimal matrix chain ordering

    // Print DP tables
    printf("\nMatrix m (Optimal Multiplication Costs):\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (j < i)
                printf("%8s", "0"); // Lower triangle is unused
            else
                printf("%8lld", m[i][j]); // Cost from i to j
        }
        printf("\n");
    }

    printf("\nMatrix s (Optimal Splits):\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (j <= i)
                printf("%4s", "0"); // Lower triangle is unused
            else
                printf("%4d", s[i][j]); // Optimal split point
        }
        printf("\n");
    }

    printf("\nOptimal Parenthesization: ");
    printOptimalParens(s, 1, n);
    printf("\n");

    clock_t start, end;
    double duration;

    start = clock();
    Matrix trivialRegular = multiplyChainTrivial(matrices, n, 0);
    end = clock();
    double durationTrivialRegular = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    start = clock();
    Matrix optimalRegular = multiplyChainOptimal(matrices, s, 1, n, 0);
    end = clock();
    double durationOptimalRegular = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    start = clock();
    Matrix trivialStrassen = multiplyChainTrivial(matrices, n, 1);
    end = clock();
    double durationTrivialStrassen = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    start = clock();
    Matrix optimalStrassen = multiplyChainOptimal(matrices, s, 1, n, 1);
    end = clock();
    double durationOptimalStrassen = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    // Print timing results
    printf("\nTiming Results (in milliseconds):\n");
    printf("1. Trivial order using Regular Multiplication: %.2f ms\n", durationTrivialRegular);
    printf("2. Trivial order using Strassen Multiplication: %.2f ms\n", durationTrivialStrassen);
    printf("3. Optimal order using Regular Multiplication: %.2f ms\n", durationOptimalRegular);
    printf("4. Optimal order using Strassen Multiplication: %.2f ms\n", durationOptimalStrassen);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        freeMatrix(matrices[i]);
    }
    free(matrices);

    for (int i = 0; i <= n; i++) {
        free(m[i]);
        free(s[i]);
    }
    free(m);
    free(s);

    freeMatrix(trivialRegular);
    freeMatrix(optimalRegular);
    freeMatrix(trivialStrassen);
    freeMatrix(optimalStrassen);

    return 0;
}