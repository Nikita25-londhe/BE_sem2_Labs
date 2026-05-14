#include<iostream>
#include<vector>
#include<queue>
#include<omp.h>
using namespace std;
class Graph
{
    int v;
    vector<int>*adj;
    public:
    Graph(int v)
    {
        this->v=v;
        adj=new vector<int>[v];
    }
    void addEdge(int u,int v)
    {   
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void printGraph()
    {
        for(int i=0;i<v;i++)
        {
            cout<<"\n"<<i<<" -> ";
            for(int j=0;j<adj[i].size();j++)
            {
                cout<<adj[i][j]<<" ";
            }
        }
        cout<<endl;
    }
    void bfs(int v)
    {
        vector<int>visited(this->v,0);
        queue<int>q;
        q.push(v);
        visited[v]=1;
        while(!q.empty())
        {
            int t=q.front();
            cout<<"\n"<<t<<" is visited";
            q.pop();
            for(int i=0;i<adj[t].size();i++)
            {
                if(!visited[adj[t][i]])
                {
                    q.push(adj[t][i]);
                    visited[adj[t][i]]=1;
                }
            }
        }

    }
    void bfsParallel(int v)
    {
        vector<int>visited(this->v,0);
        queue<int>q;
        q.push(v);
        visited[v]=1;
        while(!q.empty())
        {
            int t=q.front();
            cout<<"\n"<<t<<" is visited";
            q.pop();
            #pragma omp parallel for 
            for(int i=0;i<adj[t].size();i++)
            {
                #pragma omp critical
                {
                    if(!visited[adj[t][i]])
                    {
                        q.push(adj[t][i]);
                        visited[adj[t][i]]=1;  
                    } 
                }
            
            }
        }
    }
    void dfsUtil(int v,vector<int>&visited)
    {
        visited[v]=1;
        cout<<"\n"<<v<<" is visited";
        for(int i=0;i<adj[v].size();i++)
        {
            if(!visited[adj[v][i]])
            {
                dfsUtil(adj[v][i],visited);
            }
        }
    }

    void dfsUtilParallel(int v,vector<int>&visited)
    {
        #pragma omp critical
        {
            visited[v]=1;
            cout<<"\n"<<v<<" is visited";
        }
        for(int i=0;i<adj[v].size();i++)
        {
            if(!visited[adj[v][i]])
            {
                #pragma omp task
                dfsUtilParallel(adj[v][i],visited);
            }
        }
    }
    void dfsSequential(int v)
    {
        vector<int>visited(this->v,0);
        dfsUtil(v,visited);

    }
    void dfsParallel(int v)
    {
        vector<int>visited(this->v,0);
        dfsUtilParallel(v,visited);
    }
};
int main()
{
    int n;
    cout<<"\nEnter no of vertices:";
    cin>>n;
    Graph g(n);
    g.addEdge(0,1);
    g.addEdge(1,2);
    g.addEdge(2,3);
    g.addEdge(1,3);
    g.printGraph();
    double t1,t2;
    t1=omp_get_wtime();
    g.bfs(0);
    t2=omp_get_wtime();
    cout<<"\nTime taken by sequential BFS is "<<(t2-t1)<<" seconds";

    t1=omp_get_wtime();
    g.bfsParallel(0);
    t2=omp_get_wtime();
    cout<<"\nTime taken by Parallel BFS is "<<(t2-t1)<<" seconds";
    
    t1=omp_get_wtime();
    g.dfsSequential(0);
    t2=omp_get_wtime();
    cout<<"\nTime taken by sequential DFS is "<<(t2-t1)<<" seconds";

     t1=omp_get_wtime();
     #pragma omp parallel
     {
        #pragma omp single
        {
            g.dfsParallel(0);
        }
     }
    t2=omp_get_wtime();
    cout<<"\nTime taken by Parallel DFS is "<<(t2-t1)<<" seconds";

    return 0;
}
