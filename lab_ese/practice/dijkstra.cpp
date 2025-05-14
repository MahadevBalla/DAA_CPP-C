#include <iostream>
#include <vector>
#include <queue>
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

  vector<vector<vector<int>>> adjList;
  for(auto &edge: edges){
    int u = edge[0], v = edge[1], w = edge[2];
    adjList[u].push_back({v, w});
    adjList[v].push_back({u, w});
  }

  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  pq.push({0, src});
  while(!pq.empty()){
      auto &[currDist, u] = pq.top();
      pq.pop();
      if(currDist > dist[u]) continue;

      for(auto &edge: adjList[u]){
          int v = edge[0], w = edge[1];
          if(dist[u]+w < dist[v]){
              dist[v] = dist[u] + w;
              pq.push({dist[v], v});
          }
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