#include <iostream>
#include <vector>
using namespace std;

int main() {
  int V, E;
  cout << "No. of vertices: ";
  cin >> V;
  cout << "No. of edges: ";
  cin >> E;

  vector<vector<int>> edges;
  cout << "Enter the edges (u v w): " << endl;
  for (int i = 0; i < E; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    edges.push_back({u, v, w});
  }

  int src;
  cout << "Enter src vertex: ";
  cin >> src;
  vector<int> dist(V, INT_MAX);
  dist[src] = 0;

  for (int i = 0; i < V - 1; i++) {
    for (auto &edge : edges) {
      int u = edge[0], v = edge[1], w = edge[2];
      if (dist[u] != INT_MAX && (dist[u] + w < dist[v])) {
        dist[v] = dist[u] + w;
      }
    }
  }

  for (auto &edge : edges) {
    int u = edge[0], v = edge[1], w = edge[2];
    if (dist[u] != INT_MAX && (dist[u] + w < dist[v])) {
      cout << "Graph contains -ve wt. cycle" << endl;
    }
  }

  cout << "Shortest distances from vertex " << src
       << " to all other vertices:\n";
  for (int i = 0; i < V; i++) {
    if (dist[i] == INT_MAX)
      cout << "INF   ";
    else
      cout << dist[i] << "   ";
  }
  cout << endl;

  return 0;
}