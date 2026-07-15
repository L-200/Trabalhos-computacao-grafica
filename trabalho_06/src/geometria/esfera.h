#ifndef ESFERA_H
#define ESFERA_H

#include "objeto.h"

/*
 * Esfera representada por uma malha UV paramétrica (latitude x longitude).
 * Ao contrário do cubo, a esfera não tem faces planas: cada vértice tem
 * sua PRÓPRIA normal (a direção radial naquele ponto).
 */
class Esfera : public Objeto {
public:
    Esfera(float x, float y, float z, float raio);

protected:
    void desenharGeometria() override;

private:
    float raio;

    static const int FATIAS = 30; // divisões em longitude (em torno do eixo Y)
    static const int PILHAS = 20; // divisões em latitude (polo sul -> polo norte)
};

#endif
