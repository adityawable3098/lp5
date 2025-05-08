#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

// Sequential DFS
void sequentialDFS(int node) {
    if (visited[node]) return;
    visited[node] = true;
    cout << node << " ";
    for (int i = 0; i < graph[node].size(); i++) {
        sequentialDFS(graph[node][i]);
    }
}

// Parallel DFS
void parallelDFS(int node) {
    bool doProcess = false;

    // Critical section only modifies visited[] and sets flag
    #pragma omp critical
    {
        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";
            doProcess = true;
        }
    }

    if (!doProcess) return; // Exit if node was already visited

    // Continue with children in parallel
    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); i++) {
        int adj_node = graph[node][i];
        if (!visited[adj_node]) {
            parallelDFS(adj_node);
        }
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter No of Nodes, Edges, and Start Node: ";
    cin >> n >> m >> start_node;

    cout << "Enter Pairs of Edges:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected graph
    }

    // Sequential DFS
    #pragma omp parallel for
    for (int i = 0; i < n; i++) visited[i] = false;

    double start_seq = omp_get_wtime();
    cout << "Sequential DFS Traversal: ";
    sequentialDFS(start_node);
    double end_seq = omp_get_wtime();
    cout << "\nSequential Time: " << (end_seq - start_seq) << " seconds\n";

    // Parallel DFS
    #pragma omp parallel for
    for (int i = 0; i < n; i++) visited[i] = false;

    double start_par = omp_get_wtime();
    cout << "Parallel DFS Traversal: ";
    parallelDFS(start_node);
    double end_par = omp_get_wtime();
    cout << "\nParallel Time: " << (end_par - start_par) << " seconds\n";

    return 0;
}
