#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

// Parallel BFS function
void parallelBFS(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    cout << "Parallel BFS Traversal: ";

    while (!q.empty()) {
        int qSize = q.size();
        vector<int> nextLevel;
        vector<int> currentLevel(qSize);

        for (int i = 0; i < qSize; i++) {
            currentLevel[i] = q.front();
            q.pop();
        }

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            int node = currentLevel[i];
            #pragma omp critical
            {
                cout << node << " ";
            }

            for (size_t j = 0; j < graph[node].size(); j++) {
                int adj = graph[node][j];
                #pragma omp critical
                {
                    if (!visited[adj]) {
                        visited[adj] = true;
                        nextLevel.push_back(adj);
                    }
                }
            }
        }

        for (size_t i = 0; i < nextLevel.size(); i++) {
            q.push(nextLevel[i]);
        }
    }

    cout << endl;
}

// Sequential BFS function
void sequentialBFS(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    cout << "Sequential BFS Traversal: ";

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (size_t j = 0; j < graph[node].size(); j++) {
            int adj = graph[node][j];
            if (!visited[adj]) {
                visited[adj] = true;
                q.push(adj);
            }
        }
    }

    cout << endl;
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

    // Initialize visited array
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Measure parallel BFS time
    double start_time = omp_get_wtime();
    parallelBFS(start_node);
    double end_time = omp_get_wtime();
    cout << "Parallel BFS Time: " << (end_time - start_time) << " seconds" << endl;

    // Reset visited array
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Measure sequential BFS time
    double seq_start = omp_get_wtime();
    sequentialBFS(start_node);
    double seq_end = omp_get_wtime();
    cout << "Sequential BFS Time: " << (seq_end - seq_start) << " seconds" << endl;

    return 0;
}
