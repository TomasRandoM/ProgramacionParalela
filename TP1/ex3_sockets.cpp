#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8080

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Debe indicar 1 argumento" << endl;
        return 1;
    }

    float puntuacion;
    float numAux;
    int intAux;
    int numeroRandom;
    int n = stoi(string(argv[1]));
    int id;
    int server;
    vector<int> clientes;
    struct sockaddr_in address;
    int addresslength = sizeof(address);
    bool stop = false;

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    pid_t pid;
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            id = i;
            break;
        }
    }
    
    srand(getpid());
    
    if (pid != 0) {
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == 0) {
            cout << "Error creando socket" << endl;
            return 1;
        }
        address.sin_addr.s_addr = INADDR_ANY;
        vector<float> puntuaciones(n);
        vector<int> estados(n);
        float cartas[10] = {1, 2, 3, 4, 5, 6, 7, 0.5, 0.5, 0.5};
        int contador = 0;
        int contadorConex = 0;

        if (bind(server, (struct sockaddr*) &address, sizeof(address)) < 0) {
            cout << "Error en el bind" << endl;
            return 1;
        }

        if (listen(server, 4) < 0) {
            cout << "Error" << endl;
            return 1;
        }

        while (contadorConex != n) {
            int newSocket = accept(server, (struct sockaddr*) &address, (socklen_t*) &addresslength);
            if (newSocket < 0) {
                continue;
            }
            clientes.push_back(newSocket);
            contadorConex++;
        }

        while (stop == false) {
            for (int i = 0; i < n; i++) {
                if (estados[i] == 0) {
                    numeroRandom = rand() % 10;
                    numAux = htonl(cartas[numeroRandom]);
                    if (send(clientes[i], &numAux, sizeof(numAux), 0) < 0 ) {
                        return 1;
                    }

                    sleep(1);
                    puntuaciones[i] = puntuaciones[i] + cartas[numeroRandom];
                }
            }
            
            for (int i = 0; i < n; i++) {
                if (estados[i] == 0) {
                    ssize_t bytesrecib = recv(clientes[i], &estados[i], sizeof(estados[i]), 0);
                    estados[i] = ntohl(estados[i]);
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
            close(clientes[i]);
            close(server);
        }
    } else {
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == 0) {
            cout << "Error creando socket" << endl;
            return 1;
        }
        float puntAux;
        sleep(1);
        if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
            return 1;
        }

        if (connect(server, (struct sockaddr*) &address, sizeof(address)) < 0) {
            cout << "Error al conectar" << endl;
            return 1;
        }

        while (stop == false) {
            recv(server, &numAux, sizeof(numAux), 0);
            puntAux = ntohl(numAux);
            puntuacion = puntuacion + puntAux;
            numeroRandom = rand() % 2;
            if (puntuacion > 7.5) {
                numeroRandom = 2;
            }
            cout << "El jugador " << id << " recibió " << puntAux << " y ahora tiene " << puntuacion << " puntos, por lo que elige la acción " << numeroRandom << endl;
            intAux = htonl(numeroRandom);
            send(server, &intAux, sizeof(intAux), 0);
            if (numeroRandom != 0) {
                stop = true;
                close(server);
            }
            
        }

    }
}