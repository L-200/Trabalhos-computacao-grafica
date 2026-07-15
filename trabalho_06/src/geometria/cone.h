#ifndef CONE_H
#define CONE_H

#include "objeto.h"

/*
 * Cone reto: base circular em y=0 (local) e ápice em y=altura.
 *
 * Propriedade geométrica importante: a normal da superfície lateral é
 * CONSTANTE ao longo de cada geratriz (a linha reta que vai da borda da
 * base até o ápice, para um ângulo theta fixo) — ela só varia com theta,
 * nunca com a altura ao longo dessa linha. Isso é diferente da esfera
 * (onde a normal varia em toda e qualquer direção) e mais parecido com o
 * cubo (normal constante por região), mas aqui a "região" é uma linha,
 * não uma face inteira.
 *
 * A base também é subdividida em anéis concentrícos, não um único leque
 * saindo do centro: um disco com poucos vértices no centro perderia
 * highlights especulares concentrados que caiam fora dos vértices existentes.
 */
class Cone : public Objeto {
public:
    Cone(float x, float y, float z, float raioBase, float altura);

protected:
    void desenharGeometria() override;

private:
    float raio, altura;

    void desenharLateral() const;
    void desenharBase() const;

    static const int FATIAS = 24;  // subdivisão angular
    static const int CAMADAS = 8;  // subdivisão ao longo da altura (lateral)
    static const int ANEIS = 6;    // subdivisão radial (base)
};

#endif
