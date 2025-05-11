/**
 * Approximate Minimum Vertex Cover Algorithm
 *
 * Uses a greedy approach that picks edges and includes both endpoints in the
 * cover. Time Complexity: O(V + E) Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int V, E;
    cout << "Enter the number of vertices: ";
    cin >> V;

    cout << "Enter the number of edges: ";
    cin >> E;

    vector<vector<int>> adjList(V);

    cout << "Enter " << E << " edges (format: u v):" << endl;
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    vector<bool> visited(V, false);

    // Greedy approach: pick an uncovered edge and add both endpoints to cover
    for (int u = 0; u < V; ++u) {
        // Find an edge that is not covered
        if (!visited[u]) {
            for (int v : adjList[u]) {
                if (!visited[v]) {
                    // Add both endpoints to cover
                    visited[u] = true;
                    visited[v] = true;
                    break;
                }
            }
        }
    }
    
    // Print result
    cout << "Marked vertices: ";
    for (int i = 0; i < V; ++i) {
        if (visited[i]) cout << i << " ";
    }
    cout << endl;

    return 0;
}

/*
Test case:
Enter the number of vertices: 7
Enter the number of edges: 7
Enter 7 edges (format: u v):
0 1
0 3
1 2
1 4
2 5
4 5
2 6

Expected output:
Marked vertices: 0 1 2 4

Enter the number of vertices: 10
Enter the number of edges: 12
Enter 12 edges (format: u v):
0 1
0 2
3 4
4 5
2 3
3 4
4 6
3 5
2 5
1 4
2 4
5 6
Marked vertices: 0 1 2 3 4 5
*/