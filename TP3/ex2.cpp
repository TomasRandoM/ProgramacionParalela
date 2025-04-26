#include <iostream>
#include <pthread.h>
#include <vector>
#include <mutex>

using namespace std;

mutex mtx;
vector<int> cola;
int cont = 0;
void* usuario(void* args);
void* impresora(void* args);

int main() {
    vector<pthread_t> hilos(12);
    int r;

    for (int i = 0; i < 12; i++) {
        if (i < 2) {
            r = pthread_create(&hilos[i], NULL, impresora, (void*) (long) (i));
        } else {
            r = pthread_create(&hilos[i], NULL, usuario, (void*) (long) (i));
        }
        if (r != 0) {
            throw runtime_error("Error creando hilo.");
        }
    }

    for (int i = 0; i < 12; i++) {
        pthread_join(hilos[i], NULL);
    }
    return 0;
}

void* impresora(void* args) {
    bool stop = false;
    int numero = (int) (long) args;
    while (stop != true) {
        unique_lock<mutex> lock(mtx);
        if (!cola.empty()) {
            cont++;
            cola.pop_back();
            cout << "Impresora " << numero << " imprimiendo archivo de usuario" << endl;
        }   
        if (cont == 10) {
            stop = true;
            return NULL;
        }  
    }
    return NULL;   
}

void* usuario(void* args) {
    bool stop = false;
    while (stop != true) {
        unique_lock<mutex> lock(mtx);
        if (cola.size() < 4) {
            int id = (int) (long) args;
            stop = true;
            cola.push_back(id);
            cout << "Usuario " << id << " agregó archivo a la cola de impresión" << endl;
        }
    }
    return NULL;
}
