#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;


int main() {
    
    int pipesUsuarioCola[2];
    vector<int[2]> pipesUsuarios(10);
    int id;

    if (pipe(pipesUsuarioCola) == -1) {
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
        int contReceived = 1;
        int cont = 0;
        int confirmacion = 1;
        int trabajo;
        vector<int[2]> pipesColaImpresora(2);
        int pipesImpresoraCola[2];

        for (int i = 0; i < 2; i++) {
            if (pipe(pipesColaImpresora[i]) == -1) {
                throw runtime_error("Error creando pipe.");
            }
        
        }
        
        if (pipe(pipesImpresoraCola) == -1) {
            throw runtime_error("Error creando pipe.");
        }

        close(pipesUsuarioCola[1]);
        for (int i = 0; i < 10; i++) {
            close(pipesUsuarios[i][0]);
        }

        for (int i = 0; i < 2; i++) {
            if (pid > 0) {
                pid = fork();
                id = i;
                if (pid == 0) {
                    break;
                }
            }
        }

        if (pid > 0) {
            vector<int> cola;
            for (int i = 0; i < 2; i++) {
                close(pipesColaImpresora[i][0]);
                close(pipesImpresoraCola[1]);
            }
            
            int impresora;
            cont = 0;
            read(pipesUsuarioCola[0], &trabajo, sizeof(trabajo));
            cola.push_back(trabajo);
            cout << "Usuario " << trabajo << " agregó archivo a la cola de impresión" << endl;
            fcntl(pipesImpresoraCola[0], F_SETFL, O_NONBLOCK);
            while (cont != 10) {

                read(pipesImpresoraCola[0], &impresora, sizeof(impresora));

                if (cola.size() < 4 && contReceived < 10) {
                    read(pipesUsuarioCola[0], &trabajo, sizeof(trabajo));
                    cola.push_back(trabajo);
                    cout << "Usuario " << trabajo << " agregó archivo a la cola de impresión" << endl;
                    contReceived++;
                }

                if (!cola.empty()) {
                    if (cont == 0) {
                        trabajo = cola.back();
                        write(pipesColaImpresora[0][1], &trabajo, sizeof(trabajo));
                        cout << " " << endl;
			cola.pop_back();
                        trabajo = cola.back();
                        write(pipesColaImpresora[1][1], &trabajo, sizeof(trabajo));
                        cout << " " << endl;
			cola.pop_back();
                        cont = cont + 2;
                    } else {
                        if (impresora != -1) {
                            trabajo = cola.back();
                            write(pipesColaImpresora[impresora][1], &trabajo, sizeof(trabajo));
                            cout << " " << endl;
		            impresora = -1;
                            cont++;
                            cola.pop_back();
                        }
                    }
                } 

                if (cont == 10) {
                    trabajo = -1;
                    write(pipesColaImpresora[0][1], &trabajo, sizeof(trabajo));
                    write(pipesColaImpresora[1][1], &trabajo, sizeof(trabajo));
                }
                }
            
            for (int i = 0; i < 2; i++) {
                close(pipesColaImpresora[i][1]);
                close(pipesImpresoraCola[0]);
            }
        } else if (pid == 0) {
            for (int i = 0; i < 2; i++) {
                close(pipesColaImpresora[i][1]);
                close(pipesImpresoraCola[0]);
            }
            while (cont != 10) {
                read(pipesColaImpresora[id][0], &trabajo, sizeof(trabajo));
                
                //Si recibe -1 se le avisa que no hay más trabajos.
                if (trabajo == -1) {
                    cont = 10;
                    break;
                }
                cout << "Impresora " << id << " imprimiendo archivo de usuario"  << endl;
                write(pipesImpresoraCola[1], &id, sizeof(id));
            }
            for (int i = 0; i < 2; i++) {
                close(pipesColaImpresora[i][0]);
                close(pipesImpresoraCola[1]);
            }
        }

        close(pipesUsuarioCola[0]);
        for (int i = 0; i < 10; i++) {
            close(pipesUsuarios[i][1]);
        }
    }

    if (pid > 0) {
        int status;
        for (int i = 0; i < 12; i++) {
            wait(&status);
        }
    }
    return 0;
}
