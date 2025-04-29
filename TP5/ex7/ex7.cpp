//Programación paralela y distribuida
//Tomás Rando
//29/4/2025
//Compilación: mpic++ ex7.cpp -o ex7
//Ejecución: mpirun -np 2 ex7
#include <mpi.h>
#include <iostream>
#include <unistd.h>

using namespace std;

double t1(int i);
double t2(int j);
double t3(double a, double b, int i);
double t4(double a, double c);
double t5(double b, double c);
double ejemplo(int i, int j);

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            cout << "ERROR. Recuerde que debe utilizar 2 nodos." << endl;
        }
        MPI_Finalize();
        return 0;
    }

    double start = MPI_Wtime();
    double res = ejemplo(10, 10);
    double end = MPI_Wtime();
    if (rank == 0) {
        cout << "El resultado es: " << res << " y el tiempo de ejecución fue de: " << end - start << " segundos" << endl;
    }
    MPI_Finalize();
    return 0;
}

double ejemplo(int i, int j) {
    double a, b, c, d, e;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        a = t1(i);
        MPI_Recv(&b, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        c = t3(a, b, i);
        MPI_Send(&c, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        d = t4(a, c);
        MPI_Recv(&e, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return (d + e);
    } else {
        b = t2(j);
        MPI_Send(&b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&c, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        e = t5(b, c);
        MPI_Send(&e, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return 0;
    }
}

double t1(int i) {
    sleep(1);
    return i;
}

double t2(int j) {
    sleep(2);
    return j;
}

double t3(double a, double b, int i) {
    sleep(1);
    return (a + b + i);
}

double t4(double a, double c) {
    sleep(1);
    return a / c;
}

double t5(double b, double c) {
    sleep(2);
    return b / c;
}