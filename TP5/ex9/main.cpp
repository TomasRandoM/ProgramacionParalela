//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ main.cpp Prime.cpp -o ex9
//Ejecución: mpirun -np 5 ex9 (límite superior del rango)
#include "Prime.h"
#include <functional>
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;


int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int parametro;
    int parametro2 = 0;
    Prime prime;
    double start;
    if (argc > 2) {
        parametro = atoi(argv[1]);
        parametro++;
        parametro2 = atoi(argv[2]);
    } else if (argc > 1) {
        parametro = atoi(argv[1]);
        parametro++;
    } else {
        if (rank == 0) {
            cout << "ERROR. Debe indicar un parámetro" << endl;
        }
        MPI_Finalize();
        return 0;
    }  

    int sendPairs[size * 2];
    
    if (rank == 0) {    
        start = MPI_Wtime();
        int division = parametro / size;
        int resto = parametro % size;

        int contador = 0;
        for (int i = 0; i < size; i++) {
            sendPairs[2 * i] = contador;
            if (i < resto) {
                contador = contador + division + 1;
            } else {
                contador = contador + division;
            }

            sendPairs[2 * i + 1] = contador;
        }
    }
    
    int recvPairs[2];

    MPI_Scatter(sendPairs, 2, MPI_INT, recvPairs, 2, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> localResults;
    prime.calculatePrimeNumbers(recvPairs[0], recvPairs[1], localResults);  
    int localSize = localResults.size();

    if (rank == 0) {
        int sizes[size];
        int totalSize;
        int displs[size];
        MPI_Gather(&localSize, 1, MPI_INT, &sizes, 1, MPI_INT, 0, MPI_COMM_WORLD);

        displs[0] = 0;
        totalSize = sizes[0];
        for (int i = 1; i < size; i++) {
            totalSize = totalSize + sizes[i];
            displs[i] = displs[i - 1] + sizes[i - 1];
        }

        vector<int> results(totalSize);
        MPI_Gatherv(localResults.data(), localSize, MPI_INT, results.data(), sizes, displs, MPI_INT, 0, MPI_COMM_WORLD);
        
        double end = MPI_Wtime();

        cout << "La cantidad de números primos entre 0 y " << parametro - 1 << " es de " << results.size() << " en un tiempo de " <<  end - start << " segundos " << endl;

        if (parametro2 != 0) {
            cout << "Los números encontrados fueron: " << endl;
            
            for (int i = 0; i < results.size(); i++) {
                cout << results.at(i) << ",";
            }
            cout << endl;
        }

    } else {
        MPI_Gather(&localSize, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gatherv(localResults.data(), localSize, MPI_INT, nullptr, nullptr, nullptr, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
