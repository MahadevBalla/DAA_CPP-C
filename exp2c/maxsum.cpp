#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

#define NUM_COUNT 10000
#define OUTPUT_FILE "random_numbers.txt"
#define TIME_RESULT_FILE "timing_results.txt"

//block size 100 to 1000; incr by 100

void generate_random_numbers() {
    ofstream file(OUTPUT_FILE);
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(-1000, 1000);

    for (int i = 0; i < NUM_COUNT; i++) {
        file << dist(gen) << endl;
    }
    file.close();
}

void read_numbers(vector<int>& arr, int size) {
    ifstream file(OUTPUT_FILE);
    if (!file)
    {
        cerr << "Error opening file for reading!" << endl;
        return;
    }
    arr.resize(size);
    for (int i = 0; i < size; i++)
    {
        file >> arr[i];
    }
    file.close();
}

int bruteForce(vector<int>& arr) {
    int maxSum = INT_MIN;
    int thisSum = 0;
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        thisSum = 0;
        for (int j = i; j < n; j++) {
            thisSum += arr[j];
            if (thisSum > maxSum) {
                maxSum = thisSum;
            }
        }
    }
    return maxSum;
}

void maxCrossingSum(vector<int>& arr, int low, int mid, int high, int& ansLow, int& ansHigh, int& ansSum) {
    int leftSum = INT_MIN;
    int sum = 0;
    for (int i = mid; i >= low; i--) {
        sum += arr[i];
        if (sum > leftSum) {
            ansLow = i;
            leftSum = sum;
        }
    }

    int rightSum = INT_MIN;
    sum = 0;
    for (int i = mid + 1; i <= high; i++) {
        sum += arr[i];
        if (sum > rightSum) {
            ansHigh = i;
            rightSum = sum;
        }
    }
    ansSum = leftSum + rightSum;
}

void divideAndConquer(vector<int>& arr, int low, int high, int& ansLow, int& ansHigh, int& ansSum) {
    if (high == low) {
        ansLow = low;
        ansHigh = high;
        ansSum = arr[low];
        return;
    }
    else {
        int mid = (low + high) / 2;
        int leftLow, leftHigh, leftSum, rightLow, rightHigh, rightSum, crossLow, crossHigh, crossSum;
        divideAndConquer(arr, low, mid, leftLow, leftHigh, leftSum);
        divideAndConquer(arr, mid + 1, high, rightLow, rightHigh, rightSum);
        maxCrossingSum(arr, low, mid, high, crossLow, crossHigh, crossSum);
        if (leftSum >= rightSum && leftSum >= crossSum) {
            ansLow = leftLow;
            ansHigh = leftHigh;
            ansSum = leftSum;

        }
        else if (rightSum >= leftSum && rightSum >= crossSum) {
            ansLow = rightLow;
            ansHigh = rightHigh;
            ansSum = rightSum;
        }
        else {
            ansLow = crossLow;
            ansHigh = crossHigh;
            ansSum = crossSum;
        }
    }
}

int kadanesAlgo(vector<int>& arr) {
    int maxSum = INT_MIN;
    int thisSum = 0;

    // maxSum = thisSum > maxSum ? thisSum: maxSum;
    // thisSum = thisSum < 0 ? 0: thisSum;

    for (int j = 0; j < arr.size(); j++) {
        thisSum += arr[j];
        if (thisSum > maxSum) {
            maxSum = thisSum;
        }
        else if (thisSum < 0) {
            thisSum = 0;
        }
    }
    return maxSum;
}

void performTimingAnalysis() {
    ofstream file(TIME_RESULT_FILE);
    if (!file) {
        cerr << "Error opening time result file!" << endl;
        return;
    }
    file << "BlockSize\tBruteForce\tDivideandConquer\tKadane'sAlgorithm\n";

    for (int blockSize = 100; blockSize <= NUM_COUNT; blockSize += 100) {
        vector<int> arr;
        read_numbers(arr, blockSize);

        auto startBrute = high_resolution_clock::now();
        int bruteResult = bruteForce(arr);
        auto stopBrute = high_resolution_clock::now();
        double bruteTime = duration_cast<microseconds>(stopBrute - startBrute).count() / 1000.0;

        int ansLow, ansHigh, ansSum;
        auto startDivide = high_resolution_clock::now();
        divideAndConquer(arr, 0, arr.size() - 1, ansLow, ansHigh, ansSum);
        auto stopDivide = high_resolution_clock::now();
        double divideTime = duration_cast<microseconds>(stopDivide - startDivide).count() / 1000.0;

        auto startKadane = high_resolution_clock::now();
        int kadaneResult = kadanesAlgo(arr);
        auto stopKadane = high_resolution_clock::now();
        double kadaneTime = duration_cast<microseconds>(stopKadane - startKadane).count() / 1000.0;

        file << blockSize << "\t" << bruteTime << "\t" << divideTime << "\t" << kadaneTime << "\n";
        cout << "Block Size: " << blockSize << ", Brute Force: " << bruteTime << " ms, " << "Divide and Conquer: " << divideTime << " ms, " << "Kadane's Algorithm: " << kadaneTime << " ms" << endl;
    }
    file.close();
}

int main() {
    generate_random_numbers();
    cout << "Random nos. saved to " << OUTPUT_FILE << endl;
    performTimingAnalysis();
    cout << "Max sum & running times computed successfully and saved to " << TIME_RESULT_FILE << endl;
    return 0;
}