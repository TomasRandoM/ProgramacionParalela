#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Debe indicar 1 argumento" << endl;
        return 1;
    }

    int n = stoi(string(argv[1]));
    bool stop = false;
    pid_t pid = 2;
    int pipefd[2];
    vector<int[2]> pipes(n);
    vector<int[2]> pipesToPlayer(n);
    int id;
    float puntuacion = 0;
    int numeroRandom;

    for (int i = 0; i < n; i++) {
        if (pid > 0) {
            if (pipe(pipes[i]) == -1) {
                cout << "Error creando pipe" << endl;
                return 1;
            }
            if (pipe(pipesToPlayer[i]) == -1) {
                cout << "Error creando pipe" << endl;
                return 1;
            }
            pid = fork();
            id = i;
        } else {
            break;
        }
    }

    srand(getpid());

    if (pid > 0) {
        vector<float> puntuaciones(n);
        vector<int> estados(n);
        float cartas[10] = {1, 2, 3, 4, 5, 6, 7, 0.5, 0.5, 0.5};
        int contador = 0;
        while (stop == false) {
            for (int i = 0; i < n; i++) {
                if (estados[i] == 0) {
                    numeroRandom = rand() % 10;
                    write(pipesToPlayer[i][1], &cartas[numeroRandom], sizeof(cartas[numeroRandom]));
                    puntuaciones[i] = puntuaciones[i] + cartas[numeroRandom];
                }
            }
            
            for (int i = 0; i < n; i++) {
                if (estados[i] == 0) {
                    read(pipes[i][0], &estados[i], sizeof(estados));
                    if (estados[i] != 0) {
                        contador++;
                    }
                }
            }

            if (contador == n) {
                float max = 0;
                int winner;
                for (int i = 0; i < n; i++) {
                    if (estados[i] != 2) {
                        if ((puntuaciones[i] <= 7.5) && (puntuaciones[i] > max)) {
                            max = puntuaciones[i];
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
        for (int i = 0; i < n; i++) {
            close(pipesToPlayer[i][1]);
            close(pipesToPlayer[i][0]);
            close(pipes[i][1]);
            close(pipes[i][0]);
        }
    } else {
        float puntAux;
        while (stop == false) {
            read(pipesToPlayer[id][0], &puntAux, sizeof(puntAux));
            puntuacion = puntuacion + puntAux;
            numeroRandom = rand() % 2;
            if (puntuacion > 7.5) {
                numeroRandom = 2;
            }
            cout << "El jugador " << id << " recibió " << puntAux << " y ahora tiene " << puntuacion << " puntos, por lo que elige la acción " << numeroRandom << endl;
            write(pipes[id][1], &numeroRandom, sizeof(numeroRandom));
            if (numeroRandom != 0) {
                stop = true;
            }
            
        }
    }
    return 0;
}