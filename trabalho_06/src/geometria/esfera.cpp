#include "esfera.h"
#include <GL/gl.h>
#include <cmath>

namespace {
    const float PI = 3.14159265358979323846f;
}

Esfera::Esfera(float x, float y, float z, float raio)
    : Objeto(x, y, z, CategoriaObjeto::ESFERA), raio(raio) {}

void Esfera::desenharGeometria() {
    /*
     * A cor/material já foram aplicados em Objeto::desenhar() antes de
     * chegar aqui; esta função cuida apenas da geometria.
     */

    /*
     * Para cada "pilha" (faixa de latitude), desenhamos uma triangle strip
     * conectando os vértices dessa pilha com os da pilha seguinte.
     * Resultado: PILHAS * FATIAS * 2 triângulos cobrindo toda a esfera.
     */
    for (int i = 0; i < PILHAS; i++) {
        // phi é o ângulo de latitude: -90 graus no polo sul, +90 no polo norte
        float phi0 = -PI / 2.0f + PI * (float)i / PILHAS;
        float phi1 = -PI / 2.0f + PI * (float)(i + 1) / PILHAS;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= FATIAS; j++) {
            // theta é o ângulo de longitude, varrendo 360 graus em torno do eixo Y
            float theta = 2.0f * PI * (float)j / FATIAS;
            float cosTheta = cosf(theta);
            float sinTheta = sinf(theta);

            float phis[2] = { phi0, phi1 };
            for (int k = 0; k < 2; k++) {
                float phi = phis[k];

                /*
                 * Equações paramétricas da esfera unitária:
                 * nx = cos(phi)*cos(theta), ny = sin(phi), nz = cos(phi)*sin(theta)
                 */
                float nx = cosf(phi) * cosTheta;
                float ny = sinf(phi);
                float nz = cosf(phi) * sinTheta;

                /*
                 * Ponto importante: como (nx,ny,nz) já é um vetor unitário
                 * (por construção trigonométrica: nx²+ny²+nz² = 1), ele
                 * serve simultaneamente como NORMAL do vértice e, escalado
                 * pelo raio, como a POSIÇÃO do vértice. Isso só é verdade
                 * porque a esfera está centrada na origem local do objeto.
                 */
                glNormal3f(nx, ny, nz);
                glVertex3f(nx * raio, ny * raio, nz * raio);
            }
        }
        glEnd();
    }
}
