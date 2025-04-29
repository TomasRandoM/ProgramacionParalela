//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex6.cpp -o ex6
//Ejecución: mpirun -np 6 ex6
#include <mpi.h>
#include <iostream>
#include <cstring>

using namespace std;

int Bcast(int source, char *msg, int tam);

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    
    
    char *msg = new char[100];
    
    if (rank == 0) {
        strcpy(msg, "holas");
    }

    Bcast(0, msg, 100);

    cout << "Soy el proceso " << rank << " y mi mensaje es: " << msg << endl;
    
    delete[] msg;
    MPI_Finalize();
    return 0;
}

int Bcast(int source, char *msg, int tam) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == source) {
        for (int i = 0; i < size; i++ ) {
            if (i != rank) {
                MPI_Send(msg, tam, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {        
        MPI_Recv(msg, tam, MPI_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return 0;
    
}