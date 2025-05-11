/*
	-->	Kruskal's Algorithm for Minimum Spanning Tree (MST)
		Purpose:
		- Finds a minimum spanning tree for a connected undirected graph
		- A MST is a subset of edges that connects all vertices with minimum total edge weight

		Time Complexity: O(E log V) where E is the number of edges
		Space Complexity: O(V + E) where V is the number of vertices

		How it works:
    		1. Sort all edges in non-decreasing order of their weight
    		2. Initialize a Disjoint Set Union (DSU) data structure
    		3. Iterate over sorted edges:
    		    - If the edge connects two different components (no cycle), include it in the MST
    		    - Merge the sets (union)
    		4. Stop when you’ve added (V - 1) edges to the MST

    Time Complexity: O(E log V) [due to sorting and DSU ops]
    Space Complexity: O(V + E)
	
	--> Prim's Algorithm implementation
		Purpose:
    		- Also finds a MST from a connected undirected graph, starting from any vertex

    	How it works:
    		1. Use a priority queue (min-heap) to greedily pick the edge with the smallest weight
    		2. Start from any vertex and push all its edges into the priority queue
    		3. Repeatedly extract the minimum-weight edge and add the corresponding vertex to MST (if not already visited)
    		4. Push all unvisited neighbors of the newly added vertex into the priority queue
    		5. Continue until all vertices are included

    	Time Complexity: O(E log V) using min-heap
    	Space Complexity: O(V + E)
*/

#include <iostream>
#include <queue>
#include <climits>
#include <vector>
using namespace std;

// Disjoint Set Union for Kruskal
class DSU {
	vector<int> parent, rank;

	public:
		DSU(int V) {
			parent.resize(V);
			rank.resize(V);
			for(int i = 0; i < V; i++) {
				parent[i] = i;
				rank[i] = 1;
			}
		}

		int find(int x) {
			if(parent[x] == x) {
				return x;
			}
			return parent[x] = find(parent[x]);
		}

		void unite(int x, int y) {
			int s1 = find(x), s2 = find(y);
			if(rank[s1] > rank[s2]) {
				parent[s2] = s1;
			}
			else if(rank[s1] < rank[s2]) {
				parent[s1] = s2;
			}
			else {
				parent[s2] = s1;
				rank[s1]++;
			}

		}

};

bool comparator(vector<int>& a, vector<int>& b) {
	if(a[2]<=b[2])return true;
    return false;
}

int main() {
	int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;

    vector<vector<int>> edges;
    cout << "Enter edges (src dest weight):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    int cost = 0;
    cout << "\nMST Edges:\n";

	//Kruskal
	/*sort(edges.begin(), edges.end(), comparator);
	DSU d(V);
	int count = 0;
	for(auto& edge: edges) {
		int u = edge[0], v = edge[1], w = edge[2];
		if(d.find(u) != d.find(v)) {
			d.unite(u, v);
			cost += w;
			cout << u << "---" << v << " : " << w << endl;
			if(++count == V-1) break;
		}
	}*/
	
	//Prim
	vector<vector<vector<int>>> adjList(V);
	for(auto& edge: edges) {
		int u = edge[0], v = edge[1], w = edge[2];
		adjList[u].push_back({v, w});
		adjList[v].push_back({u, w});
	}
	
	priority_queue<vector<int>, vector<vector<int>>, greater<>> pq;
	pq.push({0, 0, -1}); // {weight, current_vertex, parent}
	vector<bool> visited(V, false);
	cost = 0;

	while(!pq.empty()) {
		auto top = pq.top(); pq.pop();
		int w = top[0], u = top[1], parent = top[2];

		if(visited[u]) continue;
		visited[u] = true;
		cost += w;

		if(parent != -1) {
			cout << parent << "--" << u << ": " << w << endl;
		}

		for(auto& edge: adjList[u]) {
			int v = edge[0], weight = edge[1];
			if(!visited[v]) {
				pq.push({weight, v, u});
			}
		}
	}
	cout << "Total MST weight: " << cost << endl;
	return 0;
}

/*
Enter number of vertices: 5
Enter the adjacency matrix (enter 0 for no edge):
0 2 0 6 0
2 0 3 8 5
0 3 0 0 7
6 8 0 0 9
0 5 7 9 0
Edge   Weight
0 - 1    2
1 - 2    3
0 - 3    6
1 - 4    5
Total MST weight: 16
*/