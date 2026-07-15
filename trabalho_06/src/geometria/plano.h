#ifndef PLANO_H
#define PLANO_H

#include "objeto.h"

/*
 * Plano: grande quad no plano XZ, representando o "chão" da cena.
 * Normal constante (0,1,0), mas SUBDIVIDIDO numa malha para que a pipeline
 * avalie corretamente a luz no centro geométrico do plano.
 */
class Plano : public Objeto {
public:
    Plano(float x, float y, float z, float tamanho);

protected:
    void desenharGeometria() override;

private:
    float tamanho;
    static const int DIVISOES = 30; // subdivisões por aresta da malha
};

#endif