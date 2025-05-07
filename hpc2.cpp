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

    for (size_t i = 0; i < graph[node].size(); i++) {
        int adj_node = graph[node][i];
        if (!visited[adj_node]) {
            sequentialDFS(adj_node);
        }
    }
}

// Parallel DFS using OpenMP (limited parallelism)
void parallelDFS(int node) {
    bool alreadyVisited = false;

    #pragma omp critical
    {
        if (visited[node]) {
            alreadyVisited = true;
        } else {
            visited[node] = true;
            cout << node << " ";
        }
    }

    if (alreadyVisited) return;

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

    cout << "Enter number of nodes: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> m;

    cout << "Enter the edges (u v) pairs:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u >= n || v >= n || u < 0 || v < 0) {
            cout << "Invalid node number. Must be between 0 and " << n-1 << ".\n";
            --i; // Retry this iteration
            continue;
        }
        graph[u].push_back(v);
        graph[v].push_back(u); // For undirected graph
    }

    cout << "Enter start node for DFS (0 to " << n-1 << "): ";
    cin >> start_node;

    // Sequential DFS timing
    for (int i = 0; i < n; i++) visited[i] = false;
    double start_seq = omp_get_wtime();
    cout << "\nSequential DFS Traversal: ";
    sequentialDFS(start_node);
    double end_seq = omp_get_wtime();
    cout << "\nTime taken (Sequential DFS): " << (end_seq - start_seq) << " seconds\n";

    // Parallel DFS timing
    for (int i = 0; i < n; i++) visited[i] = false;
    double start_par = omp_get_wtime();
    cout << "\nParallel DFS Traversal: ";
    parallelDFS(start_node);
    double end_par = omp_get_wtime();
    cout << "\nTime taken (Parallel DFS): " << (end_par - start_par) << " seconds\n";

    return 0;
}

