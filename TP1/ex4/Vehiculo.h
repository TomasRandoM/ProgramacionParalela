#ifndef VEHICULO
#define VEHICULO

#include <vector>
#include "Categoria.h"

class Vehiculo {
public:
    Vehiculo();
    std::vector<Vehiculo> registrar();
    void setId(int id);
    void setCategoria(Categoria cat);
    void setDinero(float dinero);
    int getId();
    float getDinero();
    Categoria getCategoria();
    void informar(std::vector<Vehiculo>);

private:
    int id;
    Categoria categoria;
    float dinero;
};

#endif