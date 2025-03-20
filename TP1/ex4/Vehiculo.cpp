#include <string>
#include <vector>
#include "Categoria.h"
#include "Vehiculo.h"
#include <fstream>
#include <iostream>
using namespace std;

Vehiculo::Vehiculo() {};

vector<Vehiculo> Vehiculo::registrar() {
    ifstream archivo("autopista.in");
    string sentence, dinero, id, categoria;
    int idPos, catPos, dinPos;
    vector<Vehiculo> respuesta;

    while (getline(archivo, sentence)) {
        Vehiculo v;
        idPos = sentence.find("id:");
        catPos = sentence.find("cat:");
        dinPos = sentence.find("din:");
        id = sentence.substr(idPos + 3, catPos - idPos - 4);
        categoria = sentence.substr(catPos + 4, dinPos - catPos - 5);
        dinero = sentence.substr(dinPos + 4, sentence.size() - dinPos - 4);
        
        for (char &c : dinero) {
            if (c == ',') {
                c = '.';
            }
        }

        if (categoria == "motocicleta") {
            v.setCategoria(MOTOCICLETA);
        } else if (categoria == "coche") {
            v.setCategoria(COCHE);
        } else if (categoria == "utilitario") {
            v.setCategoria(UTILITARIO);
        } else {
            v.setCategoria(PESADO);
        }
    
        v.setId(stoi(id));
        v.setDinero(stof(dinero));
        respuesta.push_back(v);
    }
    return respuesta;
}

void Vehiculo::setId(int id) {
    this -> id = id;
} 

void Vehiculo::setCategoria(Categoria cat) {
    this -> categoria = cat;
}

void Vehiculo::setDinero(float dinero) {
    this -> dinero = dinero;
}

int Vehiculo::getId() {
    return id;
}

float Vehiculo::getDinero() {
    return dinero;
}

Categoria Vehiculo::getCategoria() {
    return categoria;
}
