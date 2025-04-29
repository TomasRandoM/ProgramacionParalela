//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex12.cpp -o ex12
//Ejecución: mpirun -np 5 ex12
#include <mpi.h>
#include <iostream>
using namespace std;

void intercambiar(double x[], int proc1, int proc2);

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int M = 10;
    int N = 10;
    int x = 11;
    if (M % size != 0) {
        if (rank == 0) {
            cout << "Para esta implementación solo se acepta que el número de filas sea divisible por el número de procesos" << endl;
        }
        MPI_Finalize();
        return 0;
    }

    int A[M][N];
    //Inicializo
    if (rank == 0) {
        cout << "Matriz: " << endl;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                cout << j + i << " ";
                A[i][j] = j + i;
            }
            cout << endl;
        }
    }
    
    int totalFilas = M / size;
    int sizeRecv = totalFilas * N;
    int recv[sizeRecv];

    if (rank == 0) {
        MPI_Scatter(&A[0][0], N * totalFilas, MPI_INT, recv, N * totalFilas, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(nullptr, 0, MPI_INT, recv, N * totalFilas, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int contador = 0;
    for (int i = 0; i < sizeRecv; i++) {
        if (recv[i] == x) {
            contador++;
        }
    }
    
    int total;
    MPI_Reduce(&contador, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        cout << "La cantidad de ocurrencias de x: " << x << " es igual a: " << total << endl; 
    }
    MPI_Finalize();
    return 0;
}
