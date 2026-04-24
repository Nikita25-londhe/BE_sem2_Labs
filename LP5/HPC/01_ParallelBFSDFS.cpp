#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<int>* adj;

public:
    // Constructor
    Graph(int V) {
        this->V = V;
        adj = new vector<int>[V];
    }

    // Add edge (undirected)
    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    // -------- Parallel BFS --------
    void BFSParallel(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node;

            // Safe queue access
            #pragma omp critical
            {
                if (!q.empty()) {
                    node = q.front();
                    q.pop();
                    cout << "Visited (BFS): " << node 
                         << " by thread " << omp_get_thread_num() << endl;
                }
            }

            // Parallel processing of neighbors
            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {
                int neighbor = adj[node][i];

                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    // -------- Parallel DFS Utility --------
    void parallelDFSUtil(int node, vector<bool>& visited) {

        // Check outside critical
        if (visited[node]) return;

        #pragma omp critical
        {
            if (!visited[node]) {
                visited[node] = true;
                cout << "Visited (DFS): " << node 
                     << " by thread " << omp_get_thread_num() << endl;
            }
        }

        // Parallel exploration
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];

            if (!visited[neighbor]) {
                parallelDFSUtil(neighbor, visited);
            }
        }
    }

    // -------- Parallel DFS --------
    void DFSParallel(int start) {
        vector<bool> visited(V, false);
        parallelDFSUtil(start, visited);
    }
};

// -------- Main --------
int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "Parallel BFS starting from vertex 0:\n";
    g.BFSParallel(0);

    cout << "\nParallel DFS starting from vertex 0:\n";
    g.DFSParallel(0);
    return 0;
}
