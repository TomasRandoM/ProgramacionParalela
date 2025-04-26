#ifndef TABLERO_H
#define TABLERO_H
#include <string>
#include <vector>

class Tablero {
    public:
        Tablero();
        bool verificarNumero(int n);
        bool verificarCarton(int opcion);
        void aumentarf1();
        void aumentarf2();
        void imprimir();
        void aumentarf3();
        int getf1();
        int getf2();
        int getf3();
    private:
        int f1[9] = {0};
        int f2[9] = {0};
        int f3[9] = {0};
        int aciertosf1 = 0;
        int aciertosf2 = 0;
        int aciertosf3 = 0;
};


#endif