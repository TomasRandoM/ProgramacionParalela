#include <chrono>
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
    auto start = chrono::high_resolution_clock::now();
    double res = ejemplo(10, 10);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - start;

    cout << "El resultado es: " << res << " y el tiempo de ejecución fue de: " << time.count() << " segundos" << endl;
}

double ejemplo(int i, int j) {
    double a, b, c, d, e;
    a = t1(i);
    b = t2(j);
    c = t3(a, b, i);
    d = t4(a, c);
    e = t5(b, c);
    return (d + e);
}

double t1(int i) {
    return i;
}

double t2(int j) {
    return j;
}

double t3(double a, double b, int i) {
    sleep(1);
    return (a + b + i);
}

double t4(double a, double c) {
    return a / c;
}

double t5(double b, double c) {
    return b / c;
}