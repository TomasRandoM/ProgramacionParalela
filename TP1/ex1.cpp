#include <iostream>
#include <pthread.h>
#include <vector>
#include <unistd.h>

using namespace std;

int compartida = 0;
void* tarea1(void* identificador);
void* tarea2(void* identificador);

int main() {
    int N;
    int M;
    vector<pthread_t> hilos1(N);
    vector<pthread_t> hilos2(M);

    cout << "Ingrese N";
    cin >> N;

    cout << "Ingrese M";
    cin >> M;

    for (int i = 0; i < N; i++) {
        int r = pthread_create(&hilos1[i], NULL, tarea1, (void*) (long) i);

        if (r != 0) {
            cout << "Error creando hilo";
        }
    }

    for (int i = 0; i < M; i++) {
        int r = pthread_create(&hilos2[i], NULL, tarea2, (void*) (long) i);

        if (r != 0) {
            cout << "Error creando hilo";
        }
    }

    for (int i = 0; i < N; i++) {
        pthread_join(hilos1[i], NULL);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(hilos2[i], NULL);
    }

    cout << "Fin del programa";
    return 0;
}

void* tarea1(void* identificador) {
    cout << "Thread1 instancia " << (long) identificador;
    double randomTime = rand() / RAND_MAX;
    
    sleep(randomTime);
    compartida = compartida + 1;
    return NULL;
}

void* tarea2(void* identificador) {
    cout << "Thread2 instancia " << (long) identificador;
    double randomTime = rand() / RAND_MAX;
    
    sleep(randomTime);
    cout << compartida;
    return NULL;
}
