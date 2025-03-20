#include "Tablero.h"
#include <vector>
#include <string>
#include <iostream>

Tablero::Tablero() {
    int randomN;
    int randomN2;
    int col;
    bool condition = true;
    for (int i = 0; i < 15; i++) {
        condition = true;
        while (condition == true) {
            randomN = rand() % 10;
            if (randomN != 9) {
                col = randomN;
                randomN2 = rand() % 10;
            } else {
                col = randomN - 1;
                randomN2 = 0;
            }
            
            if (i < 5) {
                if (f1[col] != 0) {
                    continue;
                }
                f1[col] = randomN * 10 +  randomN2;
            } else if (i > 9) {
                if (f3[col] != 0) {
                    continue;
                }
                f3[col] = randomN * 10 +  randomN2;
            } else {
                if (f2[col] != 0) {
                    continue;
                }
                f2[col] = randomN * 10 +  randomN2;
            }
            condition = false;
        }
    }
}

bool Tablero::verificarNumero(int n) {
    int columna = n / 10;
    if (columna == 9) {
        columna = 8;
    }
    bool condition = false;
    if (f1[columna] == n) { 
        aumentarf1();
        condition = true;
    }
    
    if (f2[columna] == n) {
        aumentarf2();
        condition = true;
    }
    
    if (f3[columna] == n) {
        aumentarf3();
        condition = true;
    }
    return condition;
    
}

void Tablero::aumentarf1() {
    aciertosf1++;
}

void Tablero::aumentarf2() {
    aciertosf2++;
}

void Tablero::aumentarf3() {
    aciertosf3++;
}

void Tablero::imprimir() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            if (i == 0) {
                std::cout << f1[j] << " ";
            } else if (i == 1) {
                std::cout << f2[j] << " ";
            } else {
                std::cout << f3[j] << " "; 
            }
        }
        std::cout << std::endl;
    }
}

bool Tablero::verificarCarton(int opcion) {
    if (opcion == 2) {
        if (aciertosf1 == 2 || aciertosf2 == 2 || aciertosf3 == 2) {
            return true;
        }
    } else if (opcion == 3) {
        if (aciertosf1 == 3 || aciertosf2 == 3 || aciertosf3 == 3) {
            return true;
        }
    } else if (opcion == 4) {
        if (aciertosf1 == 4 || aciertosf2 == 4 || aciertosf3 == 4) {
            return true;
        }
    } else if (opcion == 5) {
        if (aciertosf1 == 5 || aciertosf2 == 5 || aciertosf3 == 5) {
            return true;
        }
    } else if (opcion == 6) {
        if (aciertosf1 == 5 && aciertosf2 == 5 && aciertosf3 == 5) {
            return true;
        }
    } else {
        return false;
    }
    return false;
}