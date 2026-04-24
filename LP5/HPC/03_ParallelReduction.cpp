/*
 * Assignment No: 02
 * Problem Statement:
 * Implement Min, Max, Sum, and Average using Parallel Reduction (OpenMP)
 * and compare with sequential performance.
 */

#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// -------- Sequential --------
int minval_sequential(const vector<int> &arr) {
    int minval = arr[0];
    for (int val : arr)
        if (val < minval)
            minval = val;
    return minval;
}

int maxval_sequential(const vector<int> &arr) {
    int maxval = arr[0];
    for (int val : arr)
        if (val > maxval)
            maxval = val;
    return maxval;
}

int sum_sequential(const vector<int> &arr) {
    int sum = 0;
    for (int val : arr)
        sum += val;
    return sum;
}

double average_sequential(const vector<int> &arr) {
    return (double)sum_sequential(arr) / arr.size();
}

// -------- Parallel (OpenMP Reduction) --------
int minval_parallel(const vector<int> &arr) {
    int minval = arr[0];

    #pragma omp parallel for reduction(min : minval)
    for (int i = 0; i < arr.size(); i++)
        if (arr[i] < minval)
            minval = arr[i];

    return minval;
}

int maxval_parallel(const vector<int> &arr) {
    int maxval = arr[0];

    #pragma omp parallel for reduction(max : maxval)
    for (int i = 0; i < arr.size(); i++)
        if (arr[i] > maxval)
            maxval = arr[i];

    return maxval;
}

int sum_parallel(const vector<int> &arr) {
    int sum = 0;

    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < arr.size(); i++)
        sum += arr[i];

    return sum;
}

double average_parallel(const vector<int> &arr) {
    return (double)sum_parallel(arr) / arr.size();
}

// -------- Main --------
int main() {
    int n = 100000;   // larger size for better comparison
    vector<int> arr(n);

    for (int i = 0; i < n; i++)
        arr[i] = i + 1;

    double start, end;

    // ---- Sequential ----
    start = omp_get_wtime();
    int min_seq = minval_sequential(arr);
    end = omp_get_wtime();
    cout << "Min (Sequential): " << min_seq << " | Time: " << end - start << " sec\n";

    start = omp_get_wtime();
    int max_seq = maxval_sequential(arr);
    end = omp_get_wtime();
    cout << "Max (Sequential): " << max_seq << " | Time: " << end - start << " sec\n";

    start = omp_get_wtime();
    int sum_seq = sum_sequential(arr);
    end = omp_get_wtime();
    cout << "Sum (Sequential): " << sum_seq << " | Time: " << end - start << " sec\n";

    start = omp_get_wtime();
    double avg_seq = average_sequential(arr);
    end = omp_get_wtime();
    cout << "Avg (Sequential): " << avg_seq << " | Time: " << end - start << " sec\n";

    // ---- Parallel ----
    start = omp_get_wtime();
    int min_par = minval_parallel(arr);
    end = omp_get_wtime();
    cout << "\nMin (Parallel): " << min_par << " | Time: " << end - start << " sec\n";

    start = omp_get_wtime();
    int max_par = maxval_parallel(arr);
    end = omp_get_wtime();
    cout << "Max (Parallel): " << max_par << " | Time: " << end - start << " sec\n";

    start = omp_get_wtime();
    int sum_par = sum_parallel(arr);
    end = omp_get_wtime();
    cout << "Sum (Parallel): " << sum_par << " | Time: " << end - start << " sec\n";

    start = omp_get_wtime();
    double avg_par = average_parallel(arr);
    end = omp_get_wtime();
    cout << "Avg (Parallel): " << avg_par << " | Time: " << end - start << " sec\n";

    return 0;
}