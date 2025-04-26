#include "Tablero.h"
#include <vector>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <fstream>
#include <algorithm> 
#include <random>

using namespace std;

mutex mtx;
int done = 0;
int players;
bool stop = false;
int valor = 99;
int premios[5] = {-1, -1, -1, -1, -1};
int premiosDados = -1; 
void* jugar(void* id);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Debe indicar 1 argumento" << endl;
        return 1;
    }
    
    int n = stoi(string(argv[1]));
    players = n;
    vector<pthread_t> jugadores(n);
    
    vector<int> listaNumeros(90);
    iota(listaNumeros.begin(), listaNumeros.end(), 1);
    random_device rd; //Esto genera semilla aleatoria
    mt19937 g(rd()); //Y esto es un generador de numeros aleatorios
    shuffle(listaNumeros.begin(), listaNumeros.end(), g);

    for (int i = 0; i < n; i++) {
        pthread_create(&jugadores[i], NULL, jugar, (void*) (long) i);
    }
    done = n;
    while (stop == false) {
        if (done == n) {            
            if (premios[premiosDados + 1] != -1) {
                if (premios[4] != -1) {
                    cout << "El jugador " << premios[premiosDados + 1] << " completó tablero" << endl; 
                    stop = true;
                    break;
                } else {
                    cout << "El jugador " << premios[premiosDados + 1] << " completó " << premiosDados + 3 << " números" << endl; 
                }
                premiosDados++;
            }

            done = 0;
            valor = listaNumeros.back();
            listaNumeros.pop_back();
        }
        if (listaNumeros.empty()) {
            stop = true;
            break;
        }
    }


    for (int i = 0; i < n; i++) {
        pthread_join(jugadores[i], NULL);
    }

    return 0;
}


void* jugar(void* id) {
    vector<Tablero> tableros;
    Tablero tab;
    int identificador = (int) ((long) id);
    for (int i = 0; i < 1; i++) {
        tableros.push_back(tab);
    }
    int revisado = 99;
    while (stop != true) {
        if (valor != 99 && revisado != valor) {
            revisado = valor;
            for (int i = 0; i < 1; i++) {
                if (tableros[i].verificarNumero(valor)) {
                    if (tableros[i].verificarCarton(premiosDados + 3)) {
                        premios[premiosDados + 1] = identificador;
                    }
                }
            }
            {
                unique_lock<mutex> lock(mtx);
                done++;
            }
        }
    }

    return NULL;
}