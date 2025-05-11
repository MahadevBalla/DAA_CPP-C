/**
 * Approximate Minimum Vertex Cover Algorithm
 * Uses a greedy approach that picks edges and includes both endpoints in the
 *cover. Time Complexity: O(V + E) Space Complexity: O(V + E)
 **/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

int main() {
  int V, E;
  int graph[MAX_VERTICES][MAX_VERTICES] = {0};
  bool visited[MAX_VERTICES] = {false};
  bool inCover[MAX_VERTICES] = {false};

  // Get input
  printf("Enter the number of vertices: ");
  scanf("%d", &V);

  printf("Enter the number of edges: ");
  scanf("%d", &E);

  printf("Enter %d edges (format: u v):\n", E);
  for (int i = 0; i < E; i++) {
    int u, v;
    scanf("%d %d", &u, &v);

    // Build undirected graph
    graph[u][v] = 1;
    graph[v][u] = 1;
  }

  // Greedy approach: pick an uncovered edge and add both endpoints to cover
  bool edgesRemain = true;
  while (edgesRemain) {
    edgesRemain = false;

    // Find an edge that is not covered
    for (int u = 0; u < V && !edgesRemain; u++) {
      for (int v = 0; v < V && !edgesRemain; v++) {
        if (graph[u][v] && !inCover[u] && !inCover[v]) {
          // Add both endpoints to cover
          inCover[u] = true;
          inCover[v] = true;
          edgesRemain = true;
        }
      }
    }
  }

  // Print result
  printf("Marked vertices: ");
  for (int i = 0; i < V; i++) {
    if (inCover[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");

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