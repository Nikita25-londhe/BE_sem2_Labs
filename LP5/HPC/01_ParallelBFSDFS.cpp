#include<iostream>
#include<vector>
#include<queue>
#include<omp.h>
using namespace std;
class Graph
{
    int V;
    vector<int> *adj;  
public:
    Graph(int V)
    {
        this->V = V;
        adj = new vector<int>[V];
    }
    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }
    void BFSParallel(int s)
    {        
        vector<bool> visited(V, false);
        queue<int> q;
        visited[s] = true;
        q.push(s);
        while (!q.empty())                      
        {
             int node;
            // Critical section to safely access queue
            #pragma omp critical
            {
                if (!q.empty()) {
                    node = q.front();
                    q.pop();
                    cout << "Visited (BFS): " << node  << endl;
                }
            }
            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) 
            {
                int neighbor = adj[node][i];

                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) 
                        {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
        }
    }
}
void parallelDFSUtil(int node, vector<bool> &visited) {

        #pragma omp critical
        {
            if (visited[node]) return;
            visited[node] = true;
            cout << "Visited (DFS): " << node << endl;
        }

        // Explore neighbors in parallel
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];

            if (!visited[neighbor]) {
                parallelDFSUtil(neighbor, visited);
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        parallelDFSUtil(start, visited);
    }
    
};  
int main()
{
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    cout << "Parallel BFS starting from vertex 0:" << endl;
    g.BFSParallel(0);
     cout << "\nParallel DFS:\n";
    g.parallelDFS(0);
    return 0;
}
