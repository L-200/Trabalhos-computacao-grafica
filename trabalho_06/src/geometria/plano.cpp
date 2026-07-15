#include "plano.h"
#include <GL/gl.h>

Plano::Plano(float x, float y, float z, float tamanho)
    : Objeto(x, y, z, CategoriaObjeto::PLANO), tamanho(tamanho) {}

void Plano::desenharGeometria() {
    float m = tamanho / 2.0f;
    float passo = tamanho / DIVISOES;

    glNormal3f(0, 1, 0); // normal única, constante em toda a malha

    for (int i = 0; i < DIVISOES; i++) {
        float x0 = -m + i * passo;
        float x1 = x0 + passo;

        for (int j = 0; j < DIVISOES; j++) {
            float z0 = -m + j * passo;
            float z1 = z0 + passo;

            glBegin(GL_QUADS);
                glVertex3f(x0, 0, z0);
                glVertex3f(x1, 0, z0);
                glVertex3f(x1, 0, z1);
                glVertex3f(x0, 0, z1);
            glEnd();
        }
    }
}