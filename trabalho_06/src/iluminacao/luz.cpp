#include "luz.h"
#include <GL/gl.h>

Luz::Luz()
    : posX(4.0f), posY(5.0f), posZ(4.0f), intensidade(1.0f) {
    /*
     * Ambiente global baixo: representa luz indireta residual, sem
     * depender de direção nenhuma.
     */
    ambiente[0] = 0.15f; ambiente[1] = 0.15f; ambiente[2] = 0.15f; ambiente[3] = 1.0f;

    /*
     * Difusa e especular "cheias" (luz branca), escaladas por
     * intensidade e zeradas conforme o modo ativo em aplicar().
     */
    difusaBase[0] = 1.0f; difusaBase[1] = 1.0f; difusaBase[2] = 1.0f; difusaBase[3] = 1.0f;
    especularBase[0] = 1.0f; especularBase[1] = 1.0f; especularBase[2] = 1.0f; especularBase[3] = 1.0f;
}

void Luz::aplicar(ModoIluminacao modo) const {
    /*
     * w = 1.0 na posição indica luz POSICIONAL (pontual, com posição no espaço)
     * diferente de w = 0.0, que indicaria luz DIRECIONAL.
     */
    float posicao[4] = { posX, posY, posZ, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, posicao);

    /*
     * O termo ambiente está sempre presente, em todos os modos.
     * Representa o "piso" de luz indireta que nunca some completamente.
     */
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);

    float difusa[4]    = {0.0f, 0.0f, 0.0f, 1.0f};
    float especular[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    if (modo == ModoIluminacao::LAMBERT || modo == ModoIluminacao::BLINN_PHONG) {
        difusa[0] = difusaBase[0] * intensidade;
        difusa[1] = difusaBase[1] * intensidade;
        difusa[2] = difusaBase[2] * intensidade;
    }
    if (modo == ModoIluminacao::BLINN_PHONG) {
        especular[0] = especularBase[0] * intensidade;
        especular[1] = especularBase[1] * intensidade;
        especular[2] = especularBase[2] * intensidade;
    }

    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
}

void Luz::mover(float dx, float dy, float dz) {
    posX += dx; posY += dy; posZ += dz;
}

void Luz::setIntensidade(float valor) {
    intensidade = valor;
    if (intensidade < 0.0f) intensidade = 0.0f;
    if (intensidade > 3.0f) intensidade = 3.0f;
}

float Luz::obterIntensidade() const {
    return intensidade;
}
