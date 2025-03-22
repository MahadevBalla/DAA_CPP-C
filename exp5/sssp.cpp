#include <bits/stdc++.h>
using namespace std;

// MinHeap implementation
class MinHeap {
private:
    vector<pair<int, int>> heap; // (key, vertex)

    void heapify(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < heap.size() && heap[left].first < heap[smallest].first) {
            smallest = left;
        }
        if (right < heap.size() && heap[right].first < heap[smallest].first) {
            smallest = right;
        }
        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

public:
    void push(pair<int, int> p) {
        heap.push_back(p);
        int i = heap.size() - 1;
        while (i > 0 && heap[(i - 1) / 2].first > heap[i].first) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    pair<int, int> pop() {
        pair<int, int> root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapify(0);
        return root;
    }

    bool empty() {
        return heap.empty();
    }
};

// Function to check if graph has negative edges
bool hasNegativeEdges(const vector<vector<pair<int, int>>>& graph, int V) {
    for (int u = 0; u < V; u++) {
        for (auto& edge : graph[u]) {
            int weight = edge.second;
            if (weight < 0) {
                return true;
            }
        }
    }
    return false;
}

// Check for negative weight cycles using Bellman-Ford
bool hasNegativeCycle(const vector<vector<pair<int, int>>>& graph, int V, int src) {
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    // Create edge list from adjacency list
    vector<vector<int>> edges;
    for (int u = 0; u < V; u++) {
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            edges.push_back({ u, v, weight });
        }
    }

    // Relax all edges |V| - 1 times
    for (int i = 1; i < V; i++) {
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int weight = edge[2];
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // Check for negative-weight cycles
    for (auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        int weight = edge[2];
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            return true;  // Found a negative cycle
        }
    }

    return false;
}

// Dijkstra's algorithm for single source
vector<int> dijkstra(const vector<vector<pair<int, int>>>& graph, int V, int src) {
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    MinHeap minHeap;
    minHeap.push(make_pair(0, src));

    while (!minHeap.empty()) {
        pair<int, int> current = minHeap.pop();
        int u = current.second;
        int distance = current.first;

        // If extracted vertex is already processed, skip
        if (distance > dist[u]) continue;

        // Process all neighbors
        for (auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            // Relaxation step
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                minHeap.push(make_pair(dist[v], v));
            }
        }
    }

    return dist;
}

// Bellman-Ford algorithm for single source
vector<int> bellmanFord(const vector<vector<pair<int, int>>>& graph, int V, int src) {
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    // Create edge list from adjacency list
    vector<vector<int>> edges;
    for (int u = 0; u < V; u++) {
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            edges.push_back({ u, v, weight });
        }
    }

    // Relax all edges |V| - 1 times
    for (int i = 1; i < V; i++) {
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int weight = edge[2];
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // Check for negative-weight cycles
    for (auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        int weight = edge[2];
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            cout << "Error: Graph contains a negative weight cycle reachable from source " << src << endl;
            return vector<int>(); // Return empty vector to indicate error
        }
    }

    return dist;
}

// Floyd-Warshall algorithm for all pairs shortest paths
vector<vector<int>> floydWarshall(const vector<vector<pair<int, int>>>& graph, int V) {
    vector<vector<int>> dist(V, vector<int>(V, INT_MAX));

    // Initialize distance matrix
    for (int i = 0; i < V; i++) {
        dist[i][i] = 0;  // Distance to self is 0
    }

    // Set direct edges
    for (int u = 0; u < V; u++) {
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            dist[u][v] = weight;
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    return dist;
}

// All Pairs Shortest Paths using Dijkstra for each vertex
vector<vector<int>> allPairsDijkstra(const vector<vector<pair<int, int>>>& graph, int V) {
    vector<vector<int>> allDistances(V, vector<int>(V));

    for (int src = 0; src < V; src++) {
        vector<int> dist = dijkstra(graph, V, src);
        allDistances[src] = dist;
    }

    return allDistances;
}

// All Pairs Shortest Paths using Bellman-Ford for each vertex
vector<vector<int>> allPairsBellmanFord(const vector<vector<pair<int, int>>>& graph, int V) {
    vector<vector<int>> allDistances(V, vector<int>(V));

    for (int src = 0; src < V; src++) {
        vector<int> dist = bellmanFord(graph, V, src);
        if (dist.empty()) {
            // If negative cycle detected, return empty matrix
            return vector<vector<int>>();
        }
        allDistances[src] = dist;
    }

    return allDistances;
}

// Print shortest paths from source to all vertices
void printShortestPaths(const vector<int>& dist, int src) {
    cout << "Shortest distances from source " << src << " to all vertices:" << endl;
    for (int i = 0; i < dist.size(); i++) {
        if (dist[i] == INT_MAX)
            cout << "Vertex " << i << ": INFINITY" << endl;
        else
            cout << "Vertex " << i << ": " << dist[i] << endl;
    }
}

// Print shortest path from source to destination
void printShortestPathToDestination(const vector<int>& dist, int src, int dest) {
    if (dist[dest] == INT_MAX)
        cout << "No path exists from source " << src << " to destination " << dest << endl;
    else
        cout << "Shortest distance from source " << src << " to destination " << dest << ": " << dist[dest] << endl;
}

// Print all pairs shortest paths
void printAllPairsShortestPaths(const vector<vector<int>>& allDistances) {
    if (allDistances.empty()) {
        cout << "Error: Graph contains a negative weight cycle." << endl;
        return;
    }

    cout << "All Pairs Shortest Paths:" << endl;
    cout << "    ";
    for (int j = 0; j < allDistances.size(); j++) {
        cout << j << "     ";
    }
    cout << endl;

    for (int i = 0; i < allDistances.size(); i++) {
        cout << i << "   ";
        for (int j = 0; j < allDistances[i].size(); j++) {
            if (allDistances[i][j] == INT_MAX)
                cout << "INF   ";
            else
                cout << allDistances[i][j] << "     ";
        }
        cout << endl;
    }
}

int main() {
    int V, E;
    cout << "Enter the number of vertices: ";
    cin >> V;
    cout << "Enter the number of edges: ";
    cin >> E;

    // Adjacency list representation of the graph
    vector<vector<pair<int, int>>> graph(V);

    // Input edges (u, v, weight)
    cout << "Enter the edges (u, v, weight):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({ v, w }); // Add edge from u to v with weight w
    }

    int src, dest, choice;
    cout << "Choose an option -" << endl
        << "1. Single Source to All Vertices\t2. Single Source to Single Destination\t3. All Pairs Shortest Paths" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    // Check if the graph has negative edges
    bool hasNegEdges = hasNegativeEdges(graph, V);

    switch (choice) {
    case 1: {
        // Single Source to All Vertices
        cout << "Enter the source vertex: ";
        cin >> src;

        if (hasNegEdges) {
            cout << "Graph contains negative edges. Using Bellman-Ford algorithm." << endl;

            // Check for negative cycles
            if (hasNegativeCycle(graph, V, src)) {
                cout << "Error: Graph contains a negative weight cycle reachable from source " << src << endl;
            }
            else {
                vector<int> dist = bellmanFord(graph, V, src);
                if (!dist.empty()) {
                    printShortestPaths(dist, src);
                }
            }
        }
        else {
            cout << "Graph has no negative edges. Using Dijkstra's algorithm." << endl;
            vector<int> dist = dijkstra(graph, V, src);
            printShortestPaths(dist, src);
        }
        break;
    }
    case 2: {
        // Single Source to Single Destination
        cout << "Enter the source vertex: ";
        cin >> src;
        cout << "Enter the destination vertex: ";
        cin >> dest;

        if (hasNegEdges) {
            cout << "Graph contains negative edges. Using Bellman-Ford algorithm." << endl;

            // Check for negative cycles
            if (hasNegativeCycle(graph, V, src)) {
                cout << "Error: Graph contains a negative weight cycle reachable from source " << src << endl;
            }
            else {
                vector<int> dist = bellmanFord(graph, V, src);
                if (!dist.empty()) {
                    printShortestPathToDestination(dist, src, dest);
                }
            }
        }
        else {
            cout << "Graph has no negative edges. Using Dijkstra's algorithm." << endl;
            vector<int> dist = dijkstra(graph, V, src);
            printShortestPathToDestination(dist, src, dest);
        }
        break;
    }
    case 3: {
        // All Pairs Shortest Paths
        if (hasNegEdges) {
            cout << "Graph contains negative edges. Using Floyd-Warshall algorithm for all pairs." << endl;
            vector<vector<int>> allDistances = floydWarshall(graph, V);
            printAllPairsShortestPaths(allDistances);
        }
        else {
            cout << "Graph has no negative edges. Using Dijkstra's algorithm for all pairs." << endl;
            vector<vector<int>> allDistances = allPairsDijkstra(graph, V);
            printAllPairsShortestPaths(allDistances);
        }
        break;
    }
    default:
        cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
        break;
    }

    return 0;
}