#include "objeto.h"
#include <GL/gl.h>

Objeto::Objeto(float x, float y, float z, CategoriaObjeto categoria)
    : posX(x), posY(y), posZ(z), categoria(categoria) {}

void Objeto::desenhar() {
    /*
     * glPushMatrix/glPopMatrix isolam a transformação deste objeto: a
     * translação aplicada aqui não "vaza" para os objetos desenhados
     * depois dele, já que a pilha de matrizes é restaurada no glPopMatrix.
     */
    glPushMatrix();
        glTranslatef(posX, posY, posZ);

        /*
         * Aplica ambient/diffuse/specular/shininess via glMaterialfv.
         * O material é obtido globalmente pela categoria, garantindo consistência.
         */
        const Material& material = GerenciadorMateriais::obterInstancia().obterMaterialAtivo(categoria);
        material.aplicar();

        /*
         * Enquanto a iluminação está desligada, usamos a componente
         * difusa do material como cor "unlit".
         */
        glColor3f(material.difR, material.difG, material.difB);

        desenharGeometria();
    glPopMatrix();
}
