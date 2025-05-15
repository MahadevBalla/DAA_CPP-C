#include <iostream>
#include <vector>
using namespace std;

#define V 4

// adjacency matrix using 2D vector
vector<vector<bool>> graph = {
    {0, 1, 1, 0},
    {1, 0, 1, 1},
    {1, 1, 0, 1},
    {0, 1, 1, 0},
};

// check whether putting a color is valid for v
bool isValid(int v, const vector<int> &color, int c) {
  for (int i = 0; i < V; i++)
    if (graph[v][i] && c == color[i])
      return false;
  return true;
}

// check graph coloring using backtracking
bool mColoring(int colors, vector<int> &color, int vertex) {
  if (vertex == V) // when all vertices are considered
    return true;

  for (int col = 1; col <= colors; col++) {
    if (isValid(vertex, color,
                col)) { // check whether color col is valid or not
      color[vertex] = col;
      if (mColoring(colors, color, vertex + 1) ==
          true) // go for additional vertices
        return true;
      color[vertex] = 0;
    }
  }
  return false; // when no colors can be assigned
}

int main() {
  int colors = 3; // Number of colors
  vector<int> color(V, 0); // make color matrix for each vertex, initially set to 0

  if (mColoring(colors, color, 0) == false) { // for vertex 0 check graph coloring
    cout << "Solution does not exist.\n";
    return 0;
  }

  cout << "Assigned Colors are: \n";
  for (int i = 0; i < V; i++)
    cout << color[i] << " ";
  cout << endl;

  return 0;
}
