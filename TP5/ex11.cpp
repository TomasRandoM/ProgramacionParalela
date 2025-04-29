//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex11.cpp -o ex11
//Ejecución: mpirun -np 5 ex11
#include <mpi.h>
#include <iostream>
using namespace std;

void func_par(double v[], int n, int rank, int np);

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int M = 10;
    if (M % size != 0) {
        if (rank == 0) {
            cout << "Para esta implementación solo se acepta que el tamaño del vector sea divisible por el número de procesos" << endl;
        }
        MPI_Finalize();
        return 0;
    }

    //Inicializo
    double A[M] = {2, 0, 5, 48, 3, 2, 5, 6, 2, 9};

    //Imprimo
    if (rank == 0) {
        cout << "Vector: " << endl;
        for (int i = 0; i < M; i++) {
            if (i != M-1) {
                cout << A[i] << ", ";
            } else {
                cout << A[i] << endl;
            }
        }
    }
    
    func_par(A, M, rank, size);
    
    MPI_Finalize();
    return 0;
}

void func_par(double v[], int n, int rank, int np) {

    int procElem = n / np;
    if (rank != 0) {
        MPI_Scatter(nullptr, procElem, MPI_DOUBLE, v, procElem, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        double auxV[procElem];
        MPI_Scatter(v, procElem, MPI_DOUBLE, &auxV, procElem, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    double max = v[0];
    int index = 0;
    for (int i = 1; i < procElem; i++) {
        if (v[i] > max) {
            index = i + (rank * procElem);
            max = v[i];
        }
    }

    double aux[2] = {max, (double) index};
    if (rank == 0) {
        int resultSize = np * 2;
        double results[resultSize];
        MPI_Gather(&aux, 2, MPI_DOUBLE, &results, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        max = results[0];
        index = results[1];
        for (int i = 2; i < resultSize - 1; i = i + 2) {
            if (results[i] > max) {
                max = results[i];
                index = (int) results[i + 1];
            }
        }
        cout << "El máximo elemento es: " << max << " y se encuentra en la posición: " << index << endl;

    } else {
        MPI_Gather(&aux, 2, MPI_DOUBLE, nullptr, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    }
    
}
