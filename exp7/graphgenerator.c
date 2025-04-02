#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MIN_VERTICES 15
#define MAX_VERTICES 20
#define MAX_CAPACITY 50
#define TEST_CASES 5

int randomInt(int low, int high) {
    return low + rand() % (high - low + 1);
}

void generateGraph(FILE* file, int V) {
    int graph[V][V];

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            graph[i][j] = 0;

    for (int i = 0; i < V - 1; i++) {
        int capacity = randomInt(1, MAX_CAPACITY);
        graph[i][i + 1] = capacity;
    }

    int extraEdges = randomInt(V, 2 * V);
    for (int i = 0; i < extraEdges; i++) {
        int u = randomInt(0, V - 1);
        int v = randomInt(0, V - 1);
        if (u != v && graph[u][v] == 0)
            graph[u][v] = randomInt(1, MAX_CAPACITY);
    }

    int src = 0, sink = V - 1;
    fprintf(file, "%d %d %d\n", V, src, sink);
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++)
            fprintf(file, "%d ", graph[i][j]);
        fprintf(file, "\n");
    }
}

int main() {
    srand(time(NULL));
    FILE* file = fopen("input.txt", "w");

    if (!file) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < TEST_CASES; i++) {
        int V = randomInt(MIN_VERTICES, MAX_VERTICES);
        generateGraph(file, V);
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Generated input.txt with %d test cases.\n", TEST_CASES);
    return 0;
}
