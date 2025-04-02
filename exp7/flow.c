#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#define MAX 21 

// DFS for Ford-Fulkerson
int dfs(int rGraph[MAX][MAX], int src, int sink, int parent[], int V, int visited[]) {
    visited[src] = 1;
    if (src == sink) return 1;

    for (int v = 0; v < V; v++) {
        if (!visited[v] && rGraph[src][v] > 0) {
            parent[v] = src;
            if (dfs(rGraph, v, sink, parent, V, visited))
                return 1;
        }
    }
    return 0;
}

int fordFulkerson(int graph[MAX][MAX], int src, int sink, int V) {
    int rGraph[MAX][MAX], parent[MAX], maxFlow = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];

    while (1) {
        int visited[MAX] = { 0 };
        memset(parent, -1, sizeof(parent));
        if (!dfs(rGraph, src, sink, parent, V, visited))
            break;
        int pathFlow = INT_MAX;

        for (int v = sink; v != src; v = parent[v])
            pathFlow = (pathFlow < rGraph[parent[v]][v]) ? pathFlow : rGraph[parent[v]][v];

        for (int v = sink; v != src; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= pathFlow;
            rGraph[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

// BFS for Edmonds-Karp
int bfs(int rGraph[MAX][MAX], int src, int sink, int parent[], int V) {
    int visited[MAX] = { 0 };
    int queue[MAX], front = 0, rear = 0;

    queue[rear++] = src;
    visited[src] = 1;
    parent[src] = -1;

    while (front < rear) {
        int u = queue[front++];

        for (int v = 0; v < V; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                parent[v] = u;
                visited[v] = 1;
                if (v == sink) return 1;
                queue[rear++] = v;
            }
        }
    }
    return 0;
}

int edmondsKarp(int graph[MAX][MAX], int src, int sink, int V) {
    int rGraph[MAX][MAX], parent[MAX], maxFlow = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];

    while (bfs(rGraph, src, sink, parent, V)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != src; v = parent[v])
            pathFlow = (pathFlow < rGraph[parent[v]][v]) ? pathFlow : rGraph[parent[v]][v];

        for (int v = sink; v != src; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= pathFlow;
            rGraph[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

// Computing max-flow and time taken by each algo
void processInputFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    int graph[MAX][MAX], V, src, sink, testCase = 1;
    while (fscanf(file, "%d %d %d", &V, &src, &sink) == 3) {
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                fscanf(file, "%d", &graph[i][j]);

        clock_t start, end;
        double cpu_time_used;

        printf("\nTest Case %d:\n", testCase++);

        start = clock();
        int ff_result = fordFulkerson(graph, src, sink, V);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Ford-Fulkerson (DFS): Max Flow = %d, Time = %.6f seconds\n", ff_result, cpu_time_used);

        start = clock();
        int ek_result = edmondsKarp(graph, src, sink, V);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Edmonds-Karp (BFS):   Max Flow = %d, Time = %.6f seconds\n", ek_result, cpu_time_used);
    }
    fclose(file);
}

int main() {
    processInputFile("input.txt");
    return 0;
}