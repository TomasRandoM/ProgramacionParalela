//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex1.cpp -o ex1
//Ejecución: mpirun -np 5 ex1
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "Hola Mundo! Desde el nodo " << rank << endl;
    MPI_Finalize();
}