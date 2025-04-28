#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int rank, size, n;
    double promedio;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    n = 10;
    promedio = 0;
    for (int i = 0; i < n; i++) {
        if (rank % 2 == 0) {
            double start, end;
            start = MPI_Wtime();
            MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 0, MPI_COMM_WORLD);
            MPI_Recv(NULL, 0, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end = MPI_Wtime();
            promedio = promedio + (end - start);
        } else {
            MPI_Recv(NULL, 0, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        promedio = promedio / n;
        cout << "La latencia es, en promedio, de: " << promedio << " segundos." << endl;
    }

    MPI_Finalize();
    return 0;
}