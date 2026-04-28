#include<iostream>
#include<vector>
#include<ctime>
#include<omp.h>
using namespace std;
void generateArray(int size,vector<int>&arr)
{
    for(int i=0;i<size;i++)
    {
        arr.push_back(rand()%10000);
    }
}
void bubbleSortSequntial(vector<int>&arr)
{
    int n=arr.size();
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}
void bubbleSortParallel(vector<int>&arr)
{
    int n=arr.size();
    for(int i=0;i<n;i++)
    {
         #pragma omp parallel for
        for(int j=i%2;j<n-1;j+=2)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}
void merge(vector<int>&arr,int left,int mid,int right)
{
    int k=right-left+1;
    vector<int>temp(k);
    int i=left;
    int j=mid+1;
    k=0;
    while(i<=mid && j<=right)
    {
        arr[i]<arr[j]?temp[k++]=arr[i++]:temp[k++]=arr[j++];
    }
    while(i<=mid)
    {
        temp[k++]=arr[i++];
    }
    while(j<=right)
    {
        temp[k++]=arr[j++];
    }
    for(int i=0;i<k;i++)
    {
        arr[left+i]=temp[i];
    }
}
void mergeSortParallel(vector<int>&arr,int left,int right)
{
    if(left<right)
    {
        int mid=(left+right)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr,left,mid);
            
            #pragma omp section
            mergeSortParallel(arr,mid+1,right);
        }
        merge(arr,left,mid,right);
    }
}
void mergeSortSequential(vector<int>&arr,int left,int right)
{
    if(left<right)
    {
        int mid=(left+right)/2;
        mergeSortSequential(arr,left,mid);
        mergeSortSequential(arr,mid+1,right);
        merge(arr,left,mid,right);
    }
}
void printArray(vector<int>arr,int n)
{
    cout<<"\nPrinting 1st "<<n<<"elements of array";
    for(int i=0;i<n;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}
int main()
{
    int size=15000;
    vector<int>a1;
    generateArray(size,a1);
    vector<int>a2=a1;

    vector<int>a3;
    generateArray(size,a3);
    vector<int>a4=a3;

    double t1,t2;
    t1=omp_get_wtime();
    bubbleSortSequntial(a1);
    t2=omp_get_wtime();
    cout<<"\nTime required for sequential bubble sort :"<<t2-t1;
    printArray(a1,10);

    t1=omp_get_wtime();
    bubbleSortParallel(a2);
    t2=omp_get_wtime();
    cout<<"\nTime required for parallel bubble sort :"<<t2-t1;
    printArray(a2,10);

    t1=omp_get_wtime();
    mergeSortSequential(a3,0,size-1);
    t2=omp_get_wtime();
    cout<<"\nTime required for sequential merge sort :"<<t2-t1;
    printArray(a3,10);

    // t1=omp_get_wtime();
    // mergeSortParallel(a4,0,size-1);
    // t2=omp_get_wtime();
    // cout<<"\nTime required for Parallel merge sort :"<<t2-t1;
    // printArray(a3,10);
    cout<<"\nLet's print time by varying no of threads in parallel merge sort\n";
    for (int t = 1; t <= 8; t *= 2)
    {
        omp_set_num_threads(t);
        double start = omp_get_wtime();
        mergeSortParallel(a4,0,size-1);
        double end = omp_get_wtime();

        cout << "Threads: " << t
            << " Time: " << end - start << endl;
    }

    // Speedup = Sequential Time / Parallel Time
    //Efficiency = Speedup / Number of Threads
    //throughput = size / time;

    return 0;
}