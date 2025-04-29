//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex13.cpp -o ex13
//Ejecución: mpirun -np 2 ex13
#include <mpi.h>
#include <iostream>
using namespace std;

void intercambiar(double x[], int proc1, int proc2);

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int M = 5;
    int N = 5;

    int A[M][N];
    //Inicializo
    if (rank == 0) {
        cout << "Matriz: " << endl;
        int aux;
        for (int i = 0; i < M; i++) {
            aux = i + 1;
            for (int j = 0; j < N; j++) {
                cout << j + i + aux << " ";
                A[i][j] = j + i + aux;
            }
            cout << endl;
        }
    }

    
    
    if (rank == 0 || rank == 1) {
        MPI_Datatype tipoColumna;
        MPI_Type_vector(N, 1, M, MPI_INT, &tipoColumna);
        MPI_Type_commit(&tipoColumna);

        if (rank == 0) {
            MPI_Send(&A[2][0], M, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Send(&A[0][2], 1, tipoColumna, 1, 0, MPI_COMM_WORLD);

        } else if (rank == 1) {
            int recv[M];
            int recv2[N];
            MPI_Recv(recv, M, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(recv2, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            cout << "Soy el proceso: " << rank << " LUEGO de intercambiar y mi vector (a) es: " << endl;
            for (int i = 0; i < M; i++) {
                if (i == M - 1) {
                    cout << recv[i] << endl;
                } else {
                    cout << recv[i] << ", ";
                }
            }

            cout << "Soy el proceso: " << rank << " LUEGO de intercambiar y mi vector (b) es: " << endl;
            for (int i = 0; i < N; i++) {
                if (i == N - 1) {
                    cout << recv2[i] << endl;
                } else {
                    cout << recv2[i] << ", ";
                }
            }
        }
        MPI_Type_free(&tipoColumna);
    }
    
    MPI_Finalize();
    return 0;
}
