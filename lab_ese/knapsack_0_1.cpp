#include <bits/stdc++.h>
using namespace std;

int max(int a, int b){
   return (a > b) ? a : b;
}

int knapSack(int W, vector<int> &wt, vector<int> &val, int n){
   int i, w;
   vector<vector<int>> K(n + 1, vector<int>(W + 1));
   for(i = 0; i <= n; i++) {
      for(w = 0; w <= W; w++) {
         if (i == 0 || w == 0)
            K[i][w] = 0;
         else if (wt[i - 1] <= w)
            K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
         else
            K[i][w] = K[i - 1][w];
      }
   }
   return K[n][W];
}

int main(){
    int n;
    cout << "Enter the no. of items: ";
    cin >> n;
    vector<int> wt(n), val(n);
    cout << "Enter the weight and profit of each item(wt val):\n";
    for (int i = 0; i < n; i++)
        cin >> wt[i] >> val[i];
    int W;
    cout << "Enter the max. capacity: ";
    cin >> W;
    cout << "Maximum Profit achieved with this knapsack: " << knapSack(W, wt, val, n);
    return 0;
}
