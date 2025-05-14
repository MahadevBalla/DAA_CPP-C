#include <vector>
#include <iostream>
using namespace std;

int main(){
    int V, E;
    cout << "No. of vertices: ";
    cin >> V;
    cout << "No. of edges: ";
    cin >> E;

    vector<vector<int>> edges(V);
    vector<int> ans;
    vector<bool> visited(V, false);
    cout << "Enter the edges(u v): " << endl;
    for (int i=0; i<E; i++){
        int u, v;
        cin >> u >> v;
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    for (int u=0; u<V; u++) {
        if(!visited[u]){
            for(auto &v: edges[u]){
                if(!visited[v]){
                    visited[u] = true;
                    visited[v] = true;
                    break;
                }
            }
        }
    }

    cout << "Minimal vertex cover includes the following nodes: ";
    for (int i=0; i<V; i++)
        if(visited[i]) cout << i << "  ";
    return 0;
}