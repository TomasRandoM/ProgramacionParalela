#include <iostream>
#include <pthread.h>
#include <vector>
#include <mutex>

using namespace std;

mutex mtx;
vector<int> cola;
void* usuario(void* args);
void* impresora(void* args);

int main() {
    vector<pthread_t> hilos(11);
    int r = pthread_create(&hilos[0], NULL, impresora, NULL);

    if (r != 0) {
        throw runtime_error("Error creando hilo.");
    }

    for (int i = 0; i < 10; i++) {
        r = pthread_create(&hilos[i + 1], NULL, usuario, (void*) (long) (i + 1));
        if (r != 0) {
            throw runtime_error("Error creando hilo.");
        }
    }

    for (int i = 0; i < 11; i++) {
        pthread_join(hilos[i], NULL);
    }
    return 0;
}

void* impresora(void* args) {
    bool stop = false;
    int cont = 0;
    while (stop != true) {
        if (!cola.empty()) {
            unique_lock<mutex> lock(mtx); //En este ejercicio más que nada sirve para proteger el cout
            cont++;
            cola.pop_back();
            cout << "Imprimiendo archivo de usuario" << endl;
            if (cont == 10) {
                stop = true;
                return NULL;
            }    
        }   
    }
    return NULL;   
}

void* usuario(void* args) {
    bool stop = false;
    while (stop != true) {
        unique_lock<mutex> lock(mtx);
        if (cola.empty()) {
            int id = (int) (long) args;
            stop = true;
            cola.push_back(id);
            cout << "Usuario " << id << " agregó archivo a la cola de impresión" << endl;
        }
    }
    return NULL;
}