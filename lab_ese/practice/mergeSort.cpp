#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

void merge(vector<int> &arr, int s, int mid, int e){
    int i=s, j=mid+1;
    vector<int> temp;
    while(i<=mid && j<=e){
        if(arr[i]<arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }
    while(i<=mid) temp.push_back(arr[i++]);
    while(j<=e) temp.push_back(arr[j++]);

    for (int i=s; i<=e; i++){
        arr[i] = temp[i - s];
    }
}

void mergeSort(vector<int> &arr, int s, int e){
    if(s<e){
        int mid = s + (e - s) / 2;
        mergeSort(arr, s, mid);
        mergeSort(arr, mid+1, e);
        merge(arr, s, mid, e);
    }
}

void printArr(vector<int> &arr){
    cout << "Elements of the array are: ";
    for(auto &it: arr){
        cout << it << " ";
    }
    cout << endl;
}

int main(){
    int n;
    cout << "Enter the no. of elements: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    auto start = high_resolution_clock::now();
    mergeSort(arr, 0, n-1);
    auto end = high_resolution_clock::now();
    double time = duration_cast<microseconds>(end - start).count();
    printArr(arr);
    cout << "Time taken: " << time << "ms" << endl;
    return 0;
}