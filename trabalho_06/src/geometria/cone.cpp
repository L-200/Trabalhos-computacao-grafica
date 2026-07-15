#include "cone.h"
#include <GL/gl.h>
#include <cmath>

namespace {
    const float PI = 3.14159265358979323846f;
}

Cone::Cone(float x, float y, float z, float raio, float altura)
    : Objeto(x, y, z, CategoriaObjeto::CONE), raio(raio), altura(altura) {}

void Cone::desenharGeometria() {
    desenharLateral();
    desenharBase();
}

void Cone::desenharLateral() const {
    /*
     * Dedução da normal:
     *
     * No corte transversal 2D (raio, altura), a geratriz vai do ponto
     * (raio, 0) na base até (0, altura) no ápice - vetor direção
     * (-raio, altura). A normal, perpendicular a essa direção e
     * apontando para FORA do sólido (afastando-se do eixo central), é a
     * rotação de 90° desse vetor: (altura, raio). Estendendo para 3D e
     * variando o ângulo theta em torno do eixo Y, a normal em cada
     * ponto da superfície lateral é:
     *     N(theta) = normalize(altura*cos(theta), raio, altura*sin(theta))
     * Note que ela não depende de v (posição ao longo da geratriz) -
     * só do ângulo. É por isso que a normal aqui é constante ao longo
     * de cada "fatia" vertical, diferente da esfera.
     */
    for (int i = 0; i < CAMADAS; i++) {
        float v0 = (float)i / CAMADAS;
        float v1 = (float)(i + 1) / CAMADAS;
        float r0 = raio * (1.0f - v0); // raio encolhe linearmente da base (v=0) ao ápice (v=1)
        float r1 = raio * (1.0f - v1);
        float y0 = v0 * altura;
        float y1 = v1 * altura;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= FATIAS; j++) {
            float theta = 2.0f * PI * j / FATIAS;
            float cosT = cosf(theta), sinT = sinf(theta);

            float nx = altura * cosT;
            float ny = raio;
            float nz = altura * sinT;
            float comprimento = sqrtf(nx*nx + ny*ny + nz*nz);
            nx /= comprimento; ny /= comprimento; nz /= comprimento;
            glNormal3f(nx, ny, nz);

            glVertex3f(r1 * cosT, y1, r1 * sinT);
            glVertex3f(r0 * cosT, y0, r0 * sinT);
        }
        glEnd();
    }
}

void Cone::desenharBase() const {
    glNormal3f(0, -1, 0); // base aponta para baixo, para fora do sólido

    // Do centro até o primeiro anel: leque de triângulos.
    float rAnel1 = raio / ANEIS;
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for (int j = 0; j <= FATIAS; j++) {
            float theta = 2.0f * PI * j / FATIAS;
            glVertex3f(rAnel1 * cosf(theta), 0, rAnel1 * sinf(theta));
        }
    glEnd();

    // Entre anéis consecutivos: tiras de triângulos.
    for (int a = 1; a < ANEIS; a++) {
        float r0 = raio * (float)a / ANEIS;
        float r1 = raio * (float)(a + 1) / ANEIS;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= FATIAS; j++) {
            float theta = 2.0f * PI * j / FATIAS;
            float cosT = cosf(theta), sinT = sinf(theta);
            glVertex3f(r1 * cosT, 0, r1 * sinT);
            glVertex3f(r0 * cosT, 0, r0 * sinT);
        }
        glEnd();
    }
}
