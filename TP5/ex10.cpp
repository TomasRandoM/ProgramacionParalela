//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex10.cpp -o ex10
//Ejecución: mpirun -np 2 ex10
#include <mpi.h>
#include <iostream>
using namespace std;

void intercambiar(double x[], int proc1, int proc2);

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size, proc1, proc2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    proc1 = 0;
    proc2 = 1;
    
    double x[10];
    if (rank == 0) {
        fill(x, x + 10, 0);
    } else {
        fill(x, x + 10, 1);
    }
    
    if (rank == proc1 || rank == proc2) {
        cout << "Soy el proceso: " << rank << " ANTES de intercambiar y mi vector es: " << endl;
        for (int i = 0; i < 10; i++) {
            if (i == (10 - 1)) {
                cout << x[i] << endl;
            } else {
                cout << x[i] << ", ";
            }
        }
    }

    intercambiar(x, proc1, proc2);

    if (rank == proc1 || rank == proc2) {
        cout << "Soy el proceso: " << rank << " LUEGO de intercambiar y mi vector es: " << endl;
        
        for (int i = 0; i < 10; i++) {
            if (i == (10 - 1)) {
                cout << x[i] << endl;
            } else {
                cout << x[i] << ", ";
            }
        }
    }

    MPI_Finalize();
    return 0;
}

void intercambiar(double x[], int proc1, int proc2) {
    double a, b, c, d, e;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == proc1) {
        MPI_Send(x, 10, MPI_DOUBLE, proc2, 0, MPI_COMM_WORLD);
        MPI_Recv(x, 10, MPI_DOUBLE, proc2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == proc2) {
        double y[10];
        copy(x, x + 10, y);
        MPI_Recv(x, 10, MPI_DOUBLE, proc1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(y, 10, MPI_DOUBLE, proc1, 0, MPI_COMM_WORLD);
    }

}