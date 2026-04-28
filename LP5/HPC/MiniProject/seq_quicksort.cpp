#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    int n = 1000000;
    vector<int> arr(n);

    srand(time(0));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100000;

    clock_t start = clock();
    quicksort(arr, 0, n - 1);
    clock_t end = clock();

    double time = double(end - start) / CLOCKS_PER_SEC;
    cout << "Sequential Time: " << time << " sec\n";

    return 0;
}