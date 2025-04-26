#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    int rank, size;

    MPI_INIT(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "Hola Mundo! Desde el nodo " << rank << endl;
    MPI_Finalize();
}