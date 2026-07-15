#include "cena.h"

Cena::~Cena() {
    for (Objeto* objeto : objetos) {
        delete objeto;
    }
}

void Cena::adicionarObjeto(Objeto* objeto) {
    objetos.push_back(objeto);
}

void Cena::desenhar() {
    for (Objeto* objeto : objetos) {
        objeto->desenhar();
    }
}

Objeto* Cena::obterObjeto(std::size_t indice) {
    if (indice >= objetos.size()) return nullptr;
    return objetos[indice];
}

std::size_t Cena::quantidade() const {
    return objetos.size();
}
