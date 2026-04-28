#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Simple quicksort using STL for local sorting
void local_sort(vector<int>& arr) {
    sort(arr.begin(), arr.end());
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 1000000;
    int chunk = n / size;

    vector<int> data;
    vector<int> local(chunk);

    // Master generates data
    if (rank == 0) {
        data.resize(n);
        srand(time(0));
        for (int i = 0; i < n; i++)
            data[i] = rand() % 100000;
    }

    double start = MPI_Wtime();

    // Scatter data to all processes
    MPI_Scatter(data.data(), chunk, MPI_INT,
                local.data(), chunk, MPI_INT,
                0, MPI_COMM_WORLD);

    // Each process sorts its chunk
    local_sort(local);

    // Gather results back
    MPI_Gather(local.data(), chunk, MPI_INT,
               data.data(), chunk, MPI_INT,
               0, MPI_COMM_WORLD);

    // Final merge at master
    if (rank == 0) {
        sort(data.begin(), data.end());

        double end = MPI_Wtime();
        cout << "Parallel Time (MPI): " << end - start << " sec\n";
    }

    MPI_Finalize();
    return 0;
}