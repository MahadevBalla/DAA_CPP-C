#include <iostream>
#include <vector>
using namespace std;

// Function to find the set cover using Approximate Greedy Set Cover algorithm
int setCover(const vector<int> &X, const vector<vector<int>> &S,
             vector<int> &output) {
  int numElements = X.size();
  int numSets = S.size();

  vector<int> U = X; // Copy of universal set (elements yet to be covered)
  vector<bool> selectedSets(numSets, false); // Track selected sets

  while (true) {
    int maxIntersectionSize = 0;
    int selectedSetIdx = -1;

    // Find set with maximum intersection with uncovered elements
    for (int i = 0; i < numSets; i++) {
      if (!selectedSets[i]) {
        int intersectionSize = 0;
        for (int j = 0; j < numElements; j++) {
          if (U[j] && S[i][j]) {
            intersectionSize++;
          }
        }
        if (intersectionSize > maxIntersectionSize) {
          maxIntersectionSize = intersectionSize;
          selectedSetIdx = i;
        }
      }
    }

    // If no set found that covers remaining elements, break
    if (selectedSetIdx == -1)
      break;

    // Mark the selected set
    selectedSets[selectedSetIdx] = true;
    output.push_back(selectedSetIdx);

    // Remove covered elements from U
    for (int j = 0; j < numElements; j++) {
      if (U[j] && S[selectedSetIdx][j]) {
        U[j] = 0; // Mark element as covered
      }
    }
  }

  return output.size();
}

int main() {
  vector<int> X = {1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1}; // Mark all elements as "uncovered" initially

  vector<vector<int>> S = {{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 1, 1, 1}};

  vector<int> output;
  int numSelectedSets = setCover(X, S, output);

  cout << "Selected Sets: ";
  for (int idx : output) {
    cout << idx << " ";
  }
  cout << endl;

  return 0;
}
