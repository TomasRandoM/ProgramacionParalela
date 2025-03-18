#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int compartida = 0;
void tarea1(int identificador);
void tarea2(int identificador);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Debe usar 2 argumentos" << endl;
        return 1;
    }

    int N = stoi(string(argv[1]));
    int M = stoi(string(argv[2]));
    pid_t pid = 2;

    for (int i = 0; i < N + M; i++) {
        if (pid > 0) {
            pid = fork();
            if (pid == 0) {
                if (i >= N) {
                    tarea2(i - N);
                } else {
                    tarea1(i);
                } 
            }
        } else {
            break;
        }
    }

    if (pid > 0) {
        for (int i = 0; i < N + M; i++) {
            wait(NULL);
        }
        cout << "Fin del programa" << endl;
    }
    
    return 0;
}

void tarea1(int identificador) {
    cout << "Thread1 instancia " << identificador << " " << endl;
    double randomTime = ((double) rand()) / RAND_MAX;
    
    sleep(randomTime);
    compartida = compartida + 1;
}

void tarea2(int identificador) {
    cout << "Thread2 instancia " << identificador << " " << endl;
    double randomTime = ((double) rand()) / RAND_MAX;
    
    sleep(randomTime);
    cout << compartida << endl;
}

//Se puede observar que no se incrementa el valor de la variable "compartida", esto sucede ya que, a diferencia de los hilos, los procesos reciben una copia de la memoria, por ende, no tienen una variable compartida entre todos.