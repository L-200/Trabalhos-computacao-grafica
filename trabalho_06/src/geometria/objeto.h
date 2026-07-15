#ifndef OBJETO_H
#define OBJETO_H

#include "gerenciador_materiais.h"

/*
 * Classe base abstrata para os objetos da cena. Centraliza a lógica comum
 * de posicionamento (translação) e material, delegando a geometria
 * específica de cada forma (esfera, cubo, plano, cone) para as
 * subclasses, via desenharGeometria().
 */
class Objeto {
public:
    Objeto(float x, float y, float z, CategoriaObjeto categoria);
    virtual ~Objeto() {}

    void desenhar(); // aplica transformação + material, depois desenharGeometria()

protected:
    virtual void desenharGeometria() = 0; // geometria pura, em coordenadas locais

    float posX, posY, posZ;
    CategoriaObjeto categoria;
};

#endif
