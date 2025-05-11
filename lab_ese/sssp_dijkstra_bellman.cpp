/*

    Bellman-Ford Algorithm in C++

    Purpose:
    - Finds the shortest distances from a single source vertex to all other
   vertices in a weighted graph (even with negative edge weights).
    - Detects negative weight cycles.

    Time Complexity:
    - O(V * E), where V = number of vertices, E = number of edges

    How it works:
    1. Initialize distances from the source to all vertices as "infinite",
   except the source itself.
    2. Relax all edges V times:
        - For each edge (u → v) with weight w,
          	if dist[u] + w < dist[v], update dist[v] = dist[u] + w.
    3. On the V-th iteration, if any edge still relaxes,
       		then the graph contains a negative weight cycle.

*/

#include <iostream>
#include <queue>
#include <climits>
#include <vector>

using namespace std;

int main() {
	int V, E;
	cout << "Enter the number of vertices: ";
	cin >> V;
	cout << "Enter the number of edges: ";
	cin >> E;

    vector<vector<int>> edges;
    cout << "Enter edges (src dest weight):" << endl;
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cin >> u >> v >> w;

		if (u < 0 || u >= V || v < 0 || v >= V) {
            cout << "Invalid vertex index: must be between 0 and " << V - 1 << endl;
            return 1;
        }
        edges.push_back({u, v, w});
    }

	int src;
    do{
		cout << "Enter source vertex: ";
    	cin >> src;
		if(src<0 || src>=V) {
			cout << "Invalid source vertex index: must be between 0 and " << V - 1 << endl;
		}
	} while (src<0 || src>=V);

	vector<int> dist(V, INT_MAX);
	dist[src] = 0;

	//Dijkstra’s
	/*vector<vector<vector<int>>> adjList(V);
	for(auto& edge: edges) {
		int u = edge[0], v = edge[1], w = edge[2];
		adjList[u].push_back({v, w});
		adjList[v].push_back({u, w});
	}
	
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
	pq.push({0, src});

	while(!pq.empty()) {
		auto [currDist, u] = pq.top();
		pq.pop();
		if (currDist > dist[u]) continue;
		
		for(auto &edge:adjList[u]) {
			int v = edge[0], w = edge[1];
			if(dist[v] > dist[u] + w) {
				dist[v] = dist[u] + w;
				pq.push({dist[v], v});
			}
		}

	}*/
	
	// Bellman-Ford
	for(int i = 0; i < V-1; i++) {
		for(auto& edge: edges) {
			int u = edge[0], v = edge[1], w = edge[2];
			if(dist[u] != INT_MAX && (dist[v] > dist[u] + w)) {
				dist[v]= dist[u] + w;
			}
		}
	}
	
	// Detect negative-weight cycles
	for(auto& edge: edges) {
	    int u = edge[0], v = edge[1], w = edge[2];
	    if(dist[u] != INT_MAX && dist[v] > dist[u] + w) {
	        cout << "Graph contains a negative-weight cycle.\n";
	        return 0;
	    }
	}

	cout << "Shortest distances from vertex " << src << ":\n";
	for (int i=0; i<V; i++) {
		if (dist[i] == INT_MAX)	cout << "INF ";
		else cout << dist[i] << " ";
	}
	cout << endl;
	return 0;
}

/*
Enter number of vertices and edges (V E): 5 5
Enter edge information (src dest weight):
1 3 2
4 3 -1
2 4 1
1 2 1
0 1 5
Enter source vertex: 0
Shortest distances from vertex 0:
0 5 6 6 7
Enter number of vertices and edges (V E): 3 3
Enter edge information (src dest weight):
0 1 5
1 2 2
0 2 10
Enter source vertex: 0
Shortest distances from vertex 0:
0 5 7
*/