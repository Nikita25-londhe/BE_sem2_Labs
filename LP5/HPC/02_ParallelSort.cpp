#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// ---------- Bubble Sort (Sequential) ----------
void bubbleSortSeq(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ---------- Bubble Sort (Parallel) ----------
void bubbleSortPar(vector<int> &arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ---------- Merge Function ----------
void merge(vector<int> &arr, int l, int m, int r) {
    vector<int> temp(r - l + 1);

    int i = l, j = m + 1, k = 0;

    while (i <= m && j <= r) {
        if (arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= m)
        temp[k++] = arr[i++];

    while (j <= r)
        temp[k++] = arr[j++];

    for (int i = l; i <= r; i++)
        arr[i] = temp[i - l];
}

// ---------- Merge Sort (Sequential) ----------
void mergeSortSeq(vector<int> &arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        mergeSortSeq(arr, l, m);
        mergeSortSeq(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// ---------- Merge Sort (Parallel) ----------
void mergeSortPar(vector<int> &arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortPar(arr, l, m);

            #pragma omp section
            mergeSortPar(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

// ---------- Utility ----------
void printArray(vector<int> arr) {
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

// ---------- Main ----------
int main() {
    vector<int> arr = {9, 5, 2, 7, 1, 6, 3, 8, 4};

    vector<int> a1 = arr;
    vector<int> a2 = arr;
    vector<int> a3 = arr;
    vector<int> a4 = arr;

    double start, end;

    // ---- Sequential Bubble ----
    start = omp_get_wtime();
    bubbleSortSeq(a1);
    end = omp_get_wtime();
    cout << "Sequential Bubble Time: " << end - start << endl;

    // ---- Parallel Bubble ----
    start = omp_get_wtime();
    bubbleSortPar(a2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Time: " << end - start << endl;

    // ---- Sequential Merge ----
    start = omp_get_wtime();
    mergeSortSeq(a3, 0, a3.size() - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Time: " << end - start << endl;

    // ---- Parallel Merge ----
    start = omp_get_wtime();
    mergeSortPar(a4, 0, a4.size() - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Time: " << end - start << endl;

    return 0;
}