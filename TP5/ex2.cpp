//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex2.cpp -o ex2
//Ejecución: mpirun -np 2 ex2
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    int rank, size, data;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        double start, end;
        start = MPI_Wtime();
        data = 1;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "[Nodo 0] Envié " << data << " hacia el nodo 1" << endl;
        MPI_Recv(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        std::cout << "[Nodo 0] Recibí " << data << " desde el nodo 1" << endl;
        end = MPI_Wtime();
        std::cout << "El tiempo de ejecución fue de " << end - start << " segundos" << endl;

    } else {
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "[Nodo 1] Recibí " << data << " desde el nodo 0" << endl;
        MPI_Send(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        std::cout << "[Nodo 1] Envié " << data << " hacia el nodo 0" << endl;
    }
    MPI_Finalize();
}