#include<iostream>
#include<vector>
#include<queue>
#include<omp.h>
using namespace std;
class Graph
{
    int V;
    vector<int>*adj;
    public:
    Graph(int V)
    {
        this->V=V;
        adj = new vector<int>[V];
    }
    void addEdge(int u,int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void printGraph()
    {
        for(int i = 0; i < V; i++)
        {
            for(auto v : adj[i])
            {
                cout << v << " ";
            }
            cout << endl;
        }
    }
    void BFS_Parallel(int start)
    {
        vector<int>visited(V,0);
        queue<int>q;
        q.push(start);
        visited[start]=1;
        while(true)
        {
            int node,flag=0;
            #pragma omp critical
            {
                if(!q.empty())
                {
                    node=q.front();
                    q.pop();
                    cout<<"\n"<<node<<" is visited by "<<omp_get_thread_num();
                }
                else
                {
                   flag=1;
                }
            }
            if(flag)
            {
                break;
            }
            int neighbour;
            #pragma omp parallel for 
            for(int i=0;i<adj[node].size();i++)
            {
                neighbour=adj[node][i];
                #pragma omp critical
                {
                    if(!visited[neighbour])
                    {
                        q.push(neighbour);
                        visited[neighbour]=1;
                    }
                }
            }
        }
    }
    void DFSUtil(int v,vector<int>&visited)
    {
        if(visited[v])
        {
            return;
        }
        #pragma omp critical
        {
            cout<<"\n"<<v<<" is visited by "<<omp_get_thread_num();
             visited[v]=1;
        }
        int neighbour;
        #pragma omp parallel for 
        for(int i=0;i<adj[v].size();i++)
        {
            neighbour=adj[v][i];
            if(!visited[neighbour])
            {
                DFSUtil(neighbour,visited);
            }
        }
    }
    void DFS_Parallel(int start)
    {
        vector<int>visited(V,0);
        DFSUtil(start,visited);
    }
};
int main()
{
    Graph g(5);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,3);
    g.addEdge(2,4);
    g.addEdge(3,4);
     g.addEdge(1,2);
    g.printGraph();
    cout<<"\nBFS Parallel:";
    g.BFS_Parallel(0);
    cout<<"\nDFS Parallel : ";
    g.DFS_Parallel(0);
    return 0;
}
