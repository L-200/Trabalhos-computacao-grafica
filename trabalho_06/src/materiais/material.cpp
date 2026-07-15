#include "material.h"
#include <GL/gl.h>

Material::Material(float ambR, float ambG, float ambB,
                    float difR, float difG, float difB,
                    float espR, float espG, float espB,
                    float brilho)
    : difR(difR), difG(difG), difB(difB), brilho(brilho) {
    ambiente[0] = ambR; ambiente[1] = ambG; ambiente[2] = ambB; ambiente[3] = 1.0f;
    difuso[0]   = difR; difuso[1]   = difG; difuso[2]   = difB; difuso[3]   = 1.0f;
    especular[0] = espR; especular[1] = espG; especular[2] = espB; especular[3] = 1.0f;
}

void Material::aplicar() const {
    /*
     * GL_FRONT: aplicamos o material apenas nas faces "de frente"
     * (front-facing), já que não usamos back-face culling.
     */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especular);
    glMaterialf(GL_FRONT, GL_SHININESS, brilho);
}

Material Material::criar(TipoMaterial tipo, float corR, float corG, float corB) {
    switch (tipo) {
        case TipoMaterial::PLASTICO:
            /*
             * Plástico: reflete boa parte da própria cor de forma difusa.
             * O brilho especular NÃO é colorido (reflete a cor da luz).
             */
            return Material(
                corR * 0.25f, corG * 0.25f, corB * 0.25f,
                corR * 0.85f, corG * 0.85f, corB * 0.85f,
                0.85f, 0.85f, 0.85f,
                32.0f
            );

        case TipoMaterial::METAL:
            /*
             * Metal: reflete pouca luz difusa, mas o brilho especular 
             * é tingido pela cor do próprio metal.
             */
            return Material(
                corR * 0.25f, corG * 0.25f, corB * 0.25f,
                corR * 0.35f, corG * 0.35f, corB * 0.35f,
                corR * 0.9f + 0.05f, corG * 0.9f + 0.05f, corB * 0.9f + 0.05f,
                128.0f
            );

        case TipoMaterial::BORRACHA:
            /*
             * Borracha: retorno predominantemente difuso,
             * especular nulo e shininess baixo.
             */
            return Material(
                corR * 0.30f, corG * 0.30f, corB * 0.30f,
                corR * 0.60f, corG * 0.60f, corB * 0.60f,
                0.05f, 0.05f, 0.05f,
                8.0f
            );

        case TipoMaterial::CERAMICA:
        default:
            /*
             * Cerâmica: difuso dominante, especular moderado e
             * shininess médio.
             */
            return Material(
                corR * 0.30f, corG * 0.30f, corB * 0.30f,
                corR * 0.75f, corG * 0.75f, corB * 0.75f,
                0.50f, 0.50f, 0.50f,
                48.0f
            );
    }
}

void Material::ajustarBrilho(float delta) {
    brilho += delta;
    /*
     * A OpenGL define o intervalo válido de shininess como [0, 128];
     * limitamos entre 1.0 e 128.0 para evitar anomalias de iluminação.
     */
    if (brilho < 1.0f) brilho = 1.0f;
    if (brilho > 128.0f) brilho = 128.0f;
}

float Material::obterBrilho() const {
    return brilho;
}
