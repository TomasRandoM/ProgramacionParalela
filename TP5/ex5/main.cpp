#include "Tablero.h"
#include <vector>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <mpi.h>
#include <fstream>
#include <algorithm> 
#include <random>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int size, rank;
    bool stop = false;
    int encontrado = 0;
    int *encontradoRecv;
    int valor[2];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2) {
        if (rank == 0) {
            cout << "ERROR. Recuerde que debe utilizar por lo menos 2 nodos." << endl;
        }
        MPI_Finalize();
        return 0;
    }
    
    if (rank == 0) {
        encontradoRecv = new int[size];
        int premios[5] = {-1, -1, -1, -1, -1};
        int premiosDados = -1;
        vector<int> listaNumeros(90);
        iota(listaNumeros.begin(), listaNumeros.end(), 1);
        random_device rd; //Esto genera semilla aleatoria
        mt19937 g(rd()); //Y esto es un generador de numeros aleatorios
        shuffle(listaNumeros.begin(), listaNumeros.end(), g);
        
        valor[1] = -1;
        while (stop == false) {
    
            if (listaNumeros.empty()) {
                stop = true;
            }

            if (premios[premiosDados + 1] != -1) {            
                if (premios[4] != -1) {
                    cout << "El jugador " << premios[premiosDados + 1] << " completó tablero" << endl; 
                    stop = true;
                } else {
                    cout << "El jugador " << premios[premiosDados + 1] << " completó " << premiosDados + 3 << " números" << endl; 
                }
                premiosDados++;
                valor[1] = premiosDados;
            }
            valor[0] = listaNumeros.back();
            listaNumeros.pop_back();
            MPI_Bcast(&valor, 2, MPI_INT, 0, MPI_COMM_WORLD);
            
            MPI_Gather(&encontrado, 1, MPI_INT, encontradoRecv, 1, MPI_INT, 0, MPI_COMM_WORLD);
            for (int i = 1; i < size; i++) {
                if (encontradoRecv[i] != 0) {
                    premios[premiosDados + 1] = i;
                    break;
                }
            }

            if (stop == true) {
                valor[0] = 99;
                MPI_Bcast(&valor, 2, MPI_INT, 0, MPI_COMM_WORLD);
            }
        }
        
        delete[] encontradoRecv;

    } else {
        vector<Tablero> tableros;
        srand(getpid());
        Tablero tab;
        for (int i = 0; i < 1; i++) {
            tableros.push_back(tab);
        }

        while (stop != true) {
            MPI_Bcast(&valor, 2, MPI_INT, 0, MPI_COMM_WORLD);
            if (valor[0] == 99) {
                stop = true;
                break;
            }

            encontrado = 0;
            if (tableros[0].verificarNumero(valor[0])) {
                if (tableros[0].verificarCarton(valor[1] + 3)) {
                    encontrado = 1;
                }
            }

            MPI_Gather(&encontrado, 1, MPI_INT, NULL, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}