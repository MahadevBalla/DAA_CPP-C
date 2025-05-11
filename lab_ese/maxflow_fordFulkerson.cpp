/*

    Ford-Fulkerson Algorithm for Maximum Flow in C

    Purpose:
    - Finds the maximum flow in a flow network from source (s) to sink (t).
    - Implements Ford-Fulkerson method using DFS to find augmenting paths.

    Time Complexity:
    - O(max_flow * E), where E is the number of edges and max_flow is the maximum flow value.

    How it works:
    1. Create a residual graph (initially same as the input graph).
    2. Find an augmenting path from source to sink using DFS.
    3. Determine the bottleneck capacity along this path.
    4. Update the residual graph by subtracting the bottleneck capacity from forward edges
       and adding it to reverse edges.
    5. Repeat steps 2-4 until no augmenting path exists.

*/

#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#define MAX_VERTICES 100
using namespace std;

// Depth-First Search to find an augmenting path in the residual graph
bool dfs(vector<vector<int>> &resGraph, int u, int t, vector<bool> &visited, vector<int> &parent) {
    visited[u] = true;
    if(u == t)  // If sink is reached, path found
        return true;

    int n = resGraph.size();
    for (int v = 0; v < n; v++) {
        // Explore unvisited adjacent vertices with positive capacity
        if (!visited[v] && resGraph[u][v] > 0) {
            parent[v] = u;  // Store path
            if (dfs(resGraph, v, t, visited, parent))
                return true;
        }
    }

    return false;  // No augmenting path found
}

// Ford-Fulkerson using DFS
int flow_dfs(vector<vector<int>> &g, int s, int t) {
    int n = g.size();
    vector<vector<int>> resGraph = g; // Residual graph initialized with original capacities
    vector<int> parent(n);  // Stores augmenting path
    int max_flow = 0;

    while (true) {
        vector<bool> visited(n, false);
        fill(parent.begin(), parent.end(), -1);

        // If no more augmenting path is found, exit
        if (!dfs(resGraph, s, t, visited, parent))
            break;

        // Find minimum capacity (bottleneck) along the path
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, resGraph[u][v]);
        }

        // Update residual capacities of the edges and reverse edges
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            resGraph[u][v] -= path_flow;
            resGraph[v][u] += path_flow;
        }

        max_flow += path_flow;  // Add path flow to total flow
    }

    return max_flow;
}

// Breadth-First Search to find an augmenting path (used in Edmonds-Karp)
bool bfs(vector<vector<int>> &resGraph, int s, int t, vector<int> &parent) {
    int n = resGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(s);             // Start from source
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            // Explore unvisited adjacent vertices with available capacity
            if (!visited[v] && resGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == t)
                    return true;  // Path to sink found
            }
        }
    }

    return false;  // No path found
}

// Ford-Fulkerson using BFS (Edmonds-Karp Algorithm)
int flow_bfs(vector<vector<int>> &g, int s, int t) {
    int n = g.size();
    vector<vector<int>> resGraph = g;  // Residual graph
    vector<int> parent(n);
    int max_flow = 0;

    // While there's an augmenting path from s to t
    while (bfs(resGraph, s, t, parent)) {
        // Find bottleneck capacity on the path
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, resGraph[u][v]);
        }

        // Update residual graph
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            resGraph[u][v] -= path_flow;
            resGraph[v][u] += path_flow;
        }

        max_flow += path_flow;  // Accumulate flow
    }

    return max_flow;
}

int main() {
    int n;
    cout << "Enter the number of vertices (2-" << MAX_VERTICES << "): ";
    cin >> n;

    if (n < 2 || n > MAX_VERTICES) {
        cout << "Invalid number of vertices." << endl;
        return 1;
    }

    vector<vector<int>> graph(n, vector<int>(n));
    cout << "Enter the adjacency matrix (" << n << " x " << n << "):" << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
            if (graph[i][j] < 0) {
                cout << "Capacity cannot be negative." << endl;
                return 1;
            }
        }
    }

    int s, t;
    cout << "Enter source and sink vertices (0-" << n - 1 << "): ";
    cin >> s >> t;

    if (s < 0 || s >= n || t < 0 || t >= n || s == t) {
        cout << "Invalid source or sink." << endl;
        return 1;
    }

    int max_flow = flow_dfs(graph, s, t);
    cout << "The maximum possible flow is " << max_flow << endl;

    return 0;
}

/*
Enter the number of vertices (2-100): 6
Enter the adjacency matrix (6 x 6):
0 16 13 0 0 0
0 0 10 12 0 0
0 4 0 0 14 0
0 0 9 0 0 20
0 0 0 7 0 4
0 0 0 0 0 0
Enter source and sink vertices (0-5): 0 5
The maximum possible flow is 23
*/