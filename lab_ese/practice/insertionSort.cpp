#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<int> &arr){
    for (int i = 1; i < arr.size(); i++){
        int key = arr[i];
        int j = i - 1;
        while(j>=0 && arr[j]>key){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(vector<int> &arr){
    int n = arr.size();
    for (int i = 0; i < n; i++){
        int minIndex = i;
        for (int j = i + 1; j < n; j++){
            if(arr[j] < arr[minIndex]){
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
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

    insertionSort(arr);
    printArr(arr);
    return 0;
}