#ifndef CENA_H
#define CENA_H

#include <vector>
#include "objeto.h"

/*
 * Agrega todos os objetos da cena e centraliza o desenho deles.
 * Também é o ponto natural para guardar o estado de
 * iluminação compartilhado.
 */
class Cena {
public:
    ~Cena(); // libera a memória de todos os objetos adicionados

    void adicionarObjeto(Objeto* objeto); // a Cena assume posse do ponteiro
    void desenhar();

    /*
     * Acesso indexado, usado pela interface para selecionar
     * qual objeto terá seu material ajustado.
     */
    Objeto* obterObjeto(std::size_t indice);
    std::size_t quantidade() const;

private:
    std::vector<Objeto*> objetos;
};

#endif
