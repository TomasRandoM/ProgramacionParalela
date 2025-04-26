#include <iostream>
#include <pthread.h>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    int pipesUsuarioCola[2];
    int pipesColaImpresora[2];
    int pipesImpresoraCola[2];
    vector<int[2]> pipesUsuarios(10);
    int id;

    if (pipe(pipesUsuarioCola) == -1) {
        throw runtime_error("Error creando pipe.");
    }

    if (pipe(pipesColaImpresora) == -1) {
        throw runtime_error("Error creando pipe.");
    }

    if (pipe(pipesImpresoraCola) == -1) {
        throw runtime_error("Error creando pipe.");
    }

    for (int i = 0; i < 10; i++) {
        if (pipe(pipesUsuarios[i]) == -1) {
            throw runtime_error("Error creando pipe.");
        }
    }
    pid_t pid = 99;
    for (int i = 0; i < 10; i++) {
        if (pid > 0) {
            pid = fork();
            id = i;
            if (pid == 0) {
                break;
            }

        }
    }

    if (pid == 0) {
        close(pipesUsuarioCola[0]);
        close(pipesColaImpresora[0]);
        close(pipesColaImpresora[1]);
        close(pipesImpresoraCola[1]);
        close(pipesImpresoraCola[0]);
        for (int i = 0; i < 10; i++) {
            close(pipesUsuarios[i][1]);
        }
        write(pipesUsuarioCola[1], &id, sizeof(id));

        int respuesta;
        read(pipesUsuarios[id][0], &respuesta, sizeof(respuesta));

        close(pipesUsuarioCola[0]);

        for (int i = 0; i < 10; i++) {
            close(pipesUsuarios[i][1]);
        }
    } else {
        int cont = 0;
        int confirmacion = 1;
        int trabajo;
        close(pipesUsuarioCola[1]);
        for (int i = 0; i < 10; i++) {
            close(pipesUsuarios[i][0]);
        }

        pid = fork();

        if (pid == 0) {
            close(pipesColaImpresora[0]);
            close(pipesImpresoraCola[1]);
            while (cont != 10) {
                if (cont == 0) {
                    read(pipesUsuarioCola[0], &trabajo, sizeof(trabajo));
                    write(pipesUsuarios[trabajo][1], &trabajo, sizeof(trabajo)); //Confirmación al usuario
                    cout << "Usuario " << trabajo << " agregó archivo a la cola de impresión" << endl;
                    write(pipesColaImpresora[1], &trabajo, sizeof(trabajo));
                } else {
                    read(pipesUsuarioCola[0], &trabajo, sizeof(trabajo)); //Leo el archivo del usuario
                    write(pipesUsuarios[trabajo][1], &trabajo, sizeof(trabajo)); //Confirmación al usuario
                    read(pipesImpresoraCola[0], &confirmacion, sizeof(confirmacion)); //Leo confirmación de que la impresora está vacía
                    cout << "Usuario " << trabajo << " agregó archivo a la cola de impresión" << endl;
                    write(pipesColaImpresora[1], &trabajo, sizeof(trabajo)); //Mando a la impresora el archivo
                }
                cont++;
            }
            close(pipesColaImpresora[1]);
            close(pipesImpresoraCola[0]);
        } else {
            close(pipesColaImpresora[1]);
            close(pipesImpresoraCola[0]);
            while (cont != 10) {
                cont++;
                read(pipesColaImpresora[0], &trabajo, sizeof(trabajo));
                cout << "Imprimiendo archivo de usuario" << endl;
                write(pipesImpresoraCola[1], &confirmacion, sizeof(confirmacion));
            }
            close(pipesColaImpresora[0]);
            close(pipesImpresoraCola[1]);

        }

        close(pipesUsuarioCola[0]);
        for (int i = 0; i < 10; i++) {
            close(pipesUsuarios[i][1]);
        }
    }

    if (pid > 0) {
        int status;
        for (int i = 0; i < 11; i++) {
            wait(&status);
        }
    }
    return 0;
}