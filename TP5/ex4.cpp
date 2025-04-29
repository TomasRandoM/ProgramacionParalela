//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex4.cpp -o ex4
//Ejecución: mpirun -np 5 ex4
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {

    int rank, size;
    int numeroRandom;
    bool stop = false;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            cout << "ERROR. Recuerde que debe utilizar por lo menos 2 nodos." << endl;
        }
        MPI_Finalize();
        return 0;
    }
    srand(getpid());

    if (rank == 0) {
        vector<float> puntuaciones(size - 1);
        vector<int> estados(size - 1);
        float cartas[10] = {1, 2, 3, 4, 5, 6, 7, 0.5, 0.5, 0.5};
        int contador = 0;
        while (stop == false) {
            for (int i = 1; i < size; i++) {
                if (estados[i - 1] == 0) {
                    numeroRandom = rand() % 10;
                    MPI_Send(&cartas[numeroRandom], 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
                    puntuaciones[i - 1] = puntuaciones[i - 1] + cartas[numeroRandom];
                }
            }
            
            for (int i = 1; i < size; i++) {
                if (estados[i - 1] == 0) {
                    MPI_Recv(&estados[i - 1], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                    if (estados[i - 1] != 0) {
                        contador++;
                    }
                }
            }

            if (contador == (size - 1)) {
                float max = 0;
                int winner;
                for (int i = 1; i < size; i++) {
                    if (estados[i - 1] != 2) {
                        if ((puntuaciones[i - 1] <= 7.5) && (puntuaciones[i - 1] > max)) {
                            max = puntuaciones[i - 1];
                            winner = i;
                        }
                    }
                }

                if (max != 0) {
                    cout << "El ganador es el jugador: " << winner << " con puntos: " << max << endl;
                } else {
                    cout << "Nadie ganó" << endl;
                }
                stop = true;
            }
            
        }
    } else {
        float puntAux;
        float puntuacion = 0;
        while (stop == false) {
            MPI_Recv(&puntAux, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
            puntuacion = puntuacion + puntAux;
            numeroRandom = rand() % 2;
            if (puntuacion > 7.5) {
                numeroRandom = 2;
            }
            cout << "El jugador " << rank << " recibió " << puntAux << " y ahora tiene " << puntuacion << " puntos, por lo que elige la acción " << numeroRandom << endl;
            MPI_Send(&numeroRandom, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            if (numeroRandom != 0) {
                stop = true;
            }
            
        }
    }
    MPI_Finalize();
    return 0;
}