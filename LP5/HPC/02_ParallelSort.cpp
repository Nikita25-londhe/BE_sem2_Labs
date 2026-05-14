#include<iostream>
#include<ctime>
#include<vector>
#include<omp.h>
using namespace std;
void prinxtArray(vector<int>arr,int n)
{
    cout<<"\n";
    for(int i=0;i<n;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<"\n";
}

vector<int> generateArray(int n)
{
    srand(time(NULL));
    vector<int>arr;
    for(int i=0;i<n;i++)
    {
        arr.push_back(rand()%10000);
    }
    return arr;
}

void bubble(vector<int>&arr)
{
    int n=arr.size();
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(arr[j]<arr[i])
            {
                swap(arr[j],arr[i]);
            }
        }
    }
}

void bubbleParallel(vector<int>&arr)
{
    int n=arr.size();
    for(int i=0;i<n;i++)
    {
        #pragma omp parallel for 
        for(int j=i%2;j<n-1;j+=2)
        {
            if(arr[j+1]<arr[j])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void merge(vector<int>&arr,int low,int mid,int high)
{
    vector<int>temp(high-low+1,0);
    int k=0;
    int i=low;
    int j=mid+1;
    while(i<=mid && j<=high)
    {
        arr[j]<arr[i]?temp[k++]=arr[j++]:temp[k++]=arr[i++];
    }
    while(i<=mid)
    {
        temp[k++]=arr[i++];
    }
    while(j<=high)
    {
        temp[k++]=arr[j++];
    }
    for(int i=0;i<temp.size();i++)
    {
        arr[low+i]=temp[i];
    }
}

void mergeSequential(vector<int>&arr,int low,int high)
{
    if(low<high)
    {
        int mid=(high+low)/2;
        mergeSequential(arr,low,mid);
        mergeSequential(arr,mid+1,high);
        merge(arr,low,mid,high);
    }
}

void mergeParallel(vector<int>&arr,int low,int high)
{
    if(low<high)
    {
        int mid=(high+low)/2;
        #pragma omp task shared(arr)
        mergeSequential(arr,low,mid);
        #pragma omp task shared(arr)
        mergeSequential(arr,mid+1,high);
       #pragma omp taskwait
        merge(arr,low,mid,high);
    }
}


int main()
{
    int n;
    cout<<"\nEnter no of elements of Array:";
    cin>>n;
    vector<int>arr1=generateArray(n);
    vector<int>arr2=arr1;
    double t1=omp_get_wtime();
    bubble(arr1);
    double t2=omp_get_wtime();
    cout<<"\nTime required for sequential bubble sort : "<<t2-t1;
    printArray(arr1,10);

    t1=omp_get_wtime();
    bubbleParallel(arr2);
    t2=omp_get_wtime();
    cout<<"\nTime required for parallel bubble sort : "<<t2-t1;
    printArray(arr2,10);


    vector<int>arr3=generateArray(n);
    vector<int>arr4=arr3;
    t1=omp_get_wtime();
    mergeSequential(arr3,0,n-1);
    t2=omp_get_wtime();
    cout<<"\nTime required for Sequntial Merge sort : "<<t2-t1;
    printArray(arr3,10);

    t1=omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            mergeParallel(arr4,0,n-1);
        }
    }
    t2=omp_get_wtime();
    cout<<"\nTime required for Parallel Merge sort : "<<t2-t1;
    printArray(arr4,10);


    return 0;
}
