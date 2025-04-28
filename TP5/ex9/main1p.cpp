#include "Prime.h"
#include <mpi.h>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;


int main(int argc, char** argv) {
    int parametro;
    int parametro2 = 0;
    Prime prime;
    if (argc > 2) {
        parametro = atoi(argv[1]);
        parametro++;
        parametro2 = atoi(argv[2]);
    } else if (argc > 1) {
        parametro = atoi(argv[1]);
        parametro++;
    } else {
        cout << "ERROR. Debe indicar un parámetro" << endl;
        return 0;
    }  
    auto start = chrono::high_resolution_clock::now();
    vector<int> results;
    prime.calculatePrimeNumbers(0, parametro, results); 

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> time = end - start;

    cout << "La cantidad de números primos entre 0 y " << parametro - 1 << " es de " << results.size() << " en un tiempo de " <<  time.count() << " segundos " << endl;

    if (parametro2 != 0) {
        cout << "Los números encontrados fueron: " << endl;
        
        for (int i = 0; i < results.size(); i++) {
            cout << results.at(i) << ",";
        }
        cout << endl;
    }

    return 0;
}
