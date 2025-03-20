#include "Categoria.h"
#include "Vehiculo.h"
#include <vector>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <fstream>

using namespace std;

queue<Vehiculo> cola;
int ticket = 0;
mutex mtx;
void* atender(void* id);

int main() {
    Vehiculo v;
    vector<Vehiculo> vector2 = v.registrar();
    Vehiculo ve1 = vector2.at(0);
    vector<pthread_t> cabinas(4);

    for (int i = 0; i < 4; i++) {
        pthread_create(&cabinas[i], NULL, atender, NULL);
    }

    while (!vector2.empty()) {
        if (cola.size() >= 15) {
            continue;
        }
        cola.push(vector2.back());
        vector2.pop_back();
    }


    for (int i = 0; i < 4; i++) {
        pthread_join(cabinas[i], NULL);
    }

    return 0;
}


void* atender(void* id) {
    sleep(1);
    Vehiculo v;
    float cobro;
    int espera;
    while (!cola.empty()) {
        {
            unique_lock<mutex> lock(mtx);
            v = cola.front();
            cola.pop();
        }
        if (v.getCategoria() == MOTOCICLETA) {
            cobro = 1.00;
            espera = 1;
        } else if (v.getCategoria() == COCHE) {
            cobro = 2.50;
            espera = 2;
        } else if (v.getCategoria() == UTILITARIO) {
            cobro = 3.25;
            espera = 3;
        } else  {
            cobro = 5.0;
            espera = 4;
        }
        v.setDinero(v.getDinero() - cobro);
        {
            unique_lock<mutex> lock(mtx);
            ofstream archivo("autopista.out", ios::app);
            archivo << "id:" << v.getId() << ";cat:" << v.getCategoria() << ";din:" << v.getDinero() << ";tick:" << ticket << endl;
            archivo.close();
            ticket++;
        }
        sleep(espera);
    }
    return NULL;
}