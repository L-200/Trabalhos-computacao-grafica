#ifndef CUBO_H
#define CUBO_H

#include "objeto.h"

/*
 * Cubo com normal CONSTANTE por face (não por vértice). Cada uma das 6
 * faces é plana, então a mesma normal vale para todos os pontos daquela
 * face — o contraste direto com a esfera (normal por vértice).
 */
class Cubo : public Objeto {
public:
    Cubo(float x, float y, float z, float tamanho);

protected:
    void desenharGeometria() override;

private:
    float tamanho;

    /*
     * Desenha uma face do cubo subdividida em uma grade de quads menores
     * (em vez de um único quad de 4 vértices). Necessário para que o
     * termo especular, calculado por vértice, tenha amostras
     * suficientes para capturar destaques estreitos (shininess alto).
     */
    void desenharFace(float baseX, float baseY, float baseZ,
                       float nx, float ny, float nz,
                       float ux, float uy, float uz,
                       float vx, float vy, float vz,
                       float meiaAresta) const;
};

#endif