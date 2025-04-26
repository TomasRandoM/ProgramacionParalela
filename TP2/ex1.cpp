#include <iostream>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <cstdlib> 

using namespace std;

int compartida = 0;
void* tarea1(void* identificador);
void* tarea2(void* identificador);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Debe usar 2 argumentos" << endl;
        return 1;
    }

    int N = stoi(string(argv[1]));
    int M = stoi(string(argv[2]));

    vector<pthread_t> hilos1(N);
    vector<pthread_t> hilos2(M);

    for (int i = 0; i < N; i++) {
        int r = pthread_create(&hilos1[i], NULL, tarea1, (void*) (long) i);

        if (r != 0) {
            cout << "Error creando hilo" << endl;
        }
    }

    for (int i = 0; i < M; i++) {
        int r = pthread_create(&hilos2[i], NULL, tarea2, (void*) (long) i);

        if (r != 0) {
            cout << "Error creando hilo" << endl;
        }
    }

    for (int i = 0; i < N; i++) {
        pthread_join(hilos1[i], NULL);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(hilos2[i], NULL);
    }

    cout << "Fin del programa" << endl;
    return 0;
}

void* tarea1(void* identificador) {
    cout << "Thread1 instancia " << (long) identificador << " " << endl;
    double randomTime = ((double) rand()) / RAND_MAX;
    
    sleep(randomTime);
    compartida = compartida + 1;
    return NULL;
}

void* tarea2(void* identificador) {
    cout << "Thread2 instancia " << (long) identificador << " " << endl;
    double randomTime = ((double) rand()) / RAND_MAX;
    
    sleep(randomTime);
    cout << compartida << endl;
    return NULL;
}
