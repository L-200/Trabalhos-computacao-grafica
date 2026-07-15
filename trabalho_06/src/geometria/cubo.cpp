#include "cubo.h"
#include <GL/gl.h>

Cubo::Cubo(float x, float y, float z, float tamanho)
    : Objeto(x, y, z, CategoriaObjeto::CUBO), tamanho(tamanho) {}

void Cubo::desenharFace(float baseX, float baseY, float baseZ,
                         float nx, float ny, float nz,
                         float ux, float uy, float uz,
                         float vx, float vy, float vz,
                         float meiaAresta) const {
    const int DIVISOES = 8; // 8x8 quads por face em vez de 1 único quad
    float passo = (2.0f * meiaAresta) / DIVISOES;

    glNormal3f(nx, ny, nz); // normal constante: vale para toda a face

    for (int i = 0; i < DIVISOES; i++) {
        float u0 = -meiaAresta + i * passo;
        float u1 = u0 + passo;

        for (int j = 0; j < DIVISOES; j++) {
            float v0 = -meiaAresta + j * passo;
            float v1 = v0 + passo;

            glBegin(GL_QUADS);
                glVertex3f(baseX + u0*ux + v0*vx, baseY + u0*uy + v0*vy, baseZ + u0*uz + v0*vz);
                glVertex3f(baseX + u1*ux + v0*vx, baseY + u1*uy + v0*vy, baseZ + u1*uz + v0*vz);
                glVertex3f(baseX + u1*ux + v1*vx, baseY + u1*uy + v1*vy, baseZ + u1*uz + v1*vz);
                glVertex3f(baseX + u0*ux + v1*vx, baseY + u0*uy + v1*vy, baseZ + u0*uz + v1*vz);
            glEnd();
        }
    }
}

void Cubo::desenharGeometria() {
    float m = tamanho / 2.0f; // meia-aresta: centraliza o cubo na origem local

    /*
     * Cada chamada abaixo desenha UMA face: origem (centro da face),
     * normal, e os dois eixos tangentes (u, v) que definem o plano da
     * face. Sem culling habilitado no projeto, a ordem de u/v não afeta
     * a aparência — a normal explícita já define a iluminação.
     */
    desenharFace(0, 0,  m,   0, 0, 1,   1, 0, 0,   0, 1, 0,   m); // frontal (+Z)
    desenharFace(0, 0, -m,   0, 0, -1,  1, 0, 0,   0, 1, 0,   m); // traseira (-Z)
    desenharFace(m, 0,  0,   1, 0, 0,   0, 0, 1,   0, 1, 0,   m); // direita (+X)
    desenharFace(-m, 0, 0,  -1, 0, 0,   0, 0, 1,   0, 1, 0,   m); // esquerda (-X)
    desenharFace(0,  m, 0,   0, 1, 0,   1, 0, 0,   0, 0, 1,   m); // superior (+Y)
    desenharFace(0, -m, 0,   0, -1, 0,  1, 0, 0,   0, 0, 1,   m); // inferior (-Y)
}