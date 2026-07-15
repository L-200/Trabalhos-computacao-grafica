// Compilar com:
// g++ main.cpp camera.cpp objeto.cpp esfera.cpp cubo.cpp cone.cpp plano.cpp cena.cpp material.cpp luz.cpp gerenciador_materiais.cpp -o trabalhoFinal -lGL -lGLU -lglut

#include <GL/glut.h>
#include <cstdio>
#include <vector>
#include <map>
#include "camera.h"
#include "cena.h"
#include "gerenciador_materiais.h"
#include "luz.h"
#include "esfera.h"
#include "cubo.h"
#include "cone.h"
#include "plano.h"

Camera camera;
Cena cena;
Luz luz;

/*
 * Ajustável em tempo real; começamos em BLINN_PHONG para
 * já demonstrar o modelo completo, mas '1'/'2'/'3' trocam livremente.
 */
ModoIluminacao modoAtual = ModoIluminacao::BLINN_PHONG;

const char* NOMES_MODO[] = { "AMBIENTE", "LAMBERT", "BLINN-PHONG" };

// Categoria atualmente selecionada para edição (shininess e troca de preset)
const CategoriaObjeto CATEGORIAS_CICLAVEIS[] = {
    CategoriaObjeto::ESFERA,
    CategoriaObjeto::CONE,
    CategoriaObjeto::CUBO,
    CategoriaObjeto::PLANO
};
size_t indiceCategoriaSelecionada = 0;
CategoriaObjeto categoriaSelecionada = CATEGORIAS_CICLAVEIS[0];

// Para acompanhar qual preset cada categoria está usando
std::map<CategoriaObjeto, size_t> indicePresetAtual = {
    {CategoriaObjeto::ESFERA, 0},
    {CategoriaObjeto::CONE, 1},
    {CategoriaObjeto::CUBO, 2},
    {CategoriaObjeto::PLANO, 3} // Vai comecar com a ceramica se mudarmos, embora o gerenciador defina ceramica diretamente
};

// Estado do mouse para controle da câmera por arrasto (drag)
bool mousePressionado = false;
int ultimoX = 0, ultimoY = 0;

int larguraJanela = 900, alturaJanela = 700;

void desenharGrade() {
    /*
     * Grade no plano XZ para dar noção de profundidade e escala à cena.
     * Estendida para cobrir o piso de 26 unidades.
     */
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    for (int i = -13; i <= 13; i++) {
        glVertex3f((float)i, 0, -13); glVertex3f((float)i, 0, 13);
        glVertex3f(-13, 0, (float)i); glVertex3f(13, 0, (float)i);
    }
    glEnd();
}

void desenharCena() {
    desenharGrade();
    cena.desenhar();
}

/*
 * Pequena esfera amarela na posição exata da luz, só para
 * referência visual. Não é um objeto da cena e não deve reagir à
 * iluminação (por isso desliga GL_LIGHTING antes de desenhar e religa
 * depois). Usa glutSolidSphere pois é apenas um indicador de interface.
 */
void desenharMarcadorLuz() {
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(luz.posX, luz.posY, luz.posZ);
        glColor3f(1.0f, 0.95f, 0.4f);
        glutSolidSphere(0.15, 12, 12);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void display() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.aplicar();
    luz.aplicar(modoAtual);
    desenharCena();
    desenharMarcadorLuz();

    glutSwapBuffers();
}

void reshape(int largura, int altura) {
    larguraJanela = largura;
    alturaJanela = altura;
    if (altura == 0) altura = 1;

    glViewport(0, 0, largura, altura);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*
     * Projeção perspectiva: 45 graus de campo de visão, aspect ratio da
     * janela, plano de corte próximo em 0.1 e distante em 100 unidades.
     */
    gluPerspective(45.0, (double)largura / (double)altura, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void mouseBotao(int botao, int estado, int x, int y) {
    // Botão esquerdo pressionado inicia o arrasto que orbita a câmera.
    if (botao == GLUT_LEFT_BUTTON) {
        mousePressionado = (estado == GLUT_DOWN);
        ultimoX = x;
        ultimoY = y;
    }

    /*
     * Roda do mouse (reportada como botões 3 e 4 em muitas builds do
     * freeglut/X11) controla o zoom.
     */
    if (botao == 3) camera.aproximar(0.5f);
    if (botao == 4) camera.aproximar(-0.5f);

    glutPostRedisplay();
}

void mouseMovimento(int x, int y) {
    if (!mousePressionado) return;

    int deltaX = x - ultimoX;
    int deltaY = y - ultimoY;
    ultimoX = x;
    ultimoY = y;

    /*
     * Sensibilidade de 0.3 grau por pixel arrastado — ajustada empiricamente.
     * Sinal de deltaY não invertido (arrastar para cima gira a câmera para cima).
     */
    camera.orbitar(deltaX * 0.3f, deltaY * 0.3f);
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y) {
    (void)x; (void)y; // não usados, mas exigidos pela assinatura do GLUT

    const float passoMovimento = 0.3f;
    switch (tecla) {
        case 'w': camera.mover(passoMovimento, 0); break;   // move para frente
        case 's': camera.mover(-passoMovimento, 0); break;  // move para trás
        case 'a': camera.mover(0, -passoMovimento); break;  // move para esquerda
        case 'd': camera.mover(0,  passoMovimento); break;  // move para direita
        case '+': camera.aproximar(0.5f); break;
        case '-': camera.aproximar(-0.5f); break;
        case 'r': case 'R': camera.resetar(); break;

        /*
         * Alternar entre os modelos de iluminação em tempo real,
         * para comparação direta.
         */
        case '1': modoAtual = ModoIluminacao::AMBIENTE;
                  printf("Modo de iluminacao: %s\n", NOMES_MODO[0]);
                  break;
        case '2': modoAtual = ModoIluminacao::LAMBERT;
                  printf("Modo de iluminacao: %s\n", NOMES_MODO[1]);
                  break;
        case '3': modoAtual = ModoIluminacao::BLINN_PHONG;
                  printf("Modo de iluminacao: %s\n", NOMES_MODO[2]);
                  break;

        // Troca o material da categoria inteira
        case '4': {
            size_t& idx = indicePresetAtual[categoriaSelecionada];
            idx = (idx + 1) % GerenciadorMateriais::obterInstancia().quantidadePresets();
            GerenciadorMateriais::obterInstancia().definirMaterialAtivo(categoriaSelecionada, idx);
            printf("%s: material alterado para %s (para toda a categoria)\n", 
                   GerenciadorMateriais::obterNomeCategoria(categoriaSelecionada), 
                   GerenciadorMateriais::obterInstancia().obterNomePreset(idx));
            printf("  -> %s\n", GerenciadorMateriais::obterInstancia().obterDescricaoPreset(idx));
            break;
        }

        // Intensidade luminosa ajustável.
        case '[': luz.setIntensidade(luz.obterIntensidade() - 0.1f);
                  printf("Intensidade da luz: %.2f\n", luz.obterIntensidade());
                  break;
        case ']': luz.setIntensidade(luz.obterIntensidade() + 0.1f);
                  printf("Intensidade da luz: %.2f\n", luz.obterIntensidade());
                  break;

        // Tab (código ASCII 9): alterna qual categoria está selecionada
        case 9:
            indiceCategoriaSelecionada = (indiceCategoriaSelecionada + 1) % 4; // Agora 4 categorias
            categoriaSelecionada = CATEGORIAS_CICLAVEIS[indiceCategoriaSelecionada];
            printf("Categoria selecionada: %s\n", GerenciadorMateriais::obterNomeCategoria(categoriaSelecionada));
            break;

        /*
         * Brilho especular (shininess) ajustável, na categoria
         * atualmente selecionada.
         */
        case ',': {
            GerenciadorMateriais::obterInstancia().ajustarBrilho(categoriaSelecionada, -4.0f);
            printf("%s - shininess: %.1f\n", 
                   GerenciadorMateriais::obterNomeCategoria(categoriaSelecionada), 
                   GerenciadorMateriais::obterInstancia().obterBrilho(categoriaSelecionada));
            break;
        }
        case '.': {
            GerenciadorMateriais::obterInstancia().ajustarBrilho(categoriaSelecionada, 4.0f);
            printf("%s - shininess: %.1f\n", 
                   GerenciadorMateriais::obterNomeCategoria(categoriaSelecionada), 
                   GerenciadorMateriais::obterInstancia().obterBrilho(categoriaSelecionada));
            break;
        }

        case 27: exit(0); break; // ESC encerra o programa
    }
    glutPostRedisplay();
}

void teclasEspeciais(int tecla, int x, int y) {
    (void)x; (void)y;

    const float passoLuz = 0.3f;
    switch (tecla) {
        // Posição da luz ajustável.
        case GLUT_KEY_LEFT:  luz.mover(-passoLuz, 0, 0); break;
        case GLUT_KEY_RIGHT: luz.mover( passoLuz, 0, 0); break;
        case GLUT_KEY_UP:    luz.mover(0, 0, -passoLuz); break;
        case GLUT_KEY_DOWN:  luz.mover(0, 0,  passoLuz); break;
        case GLUT_KEY_PAGE_UP:   luz.mover(0,  passoLuz, 0); break;
        case GLUT_KEY_PAGE_DOWN: luz.mover(0, -passoLuz, 0); break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    printf("========================================\n");
    printf(" Trabalho Final CG - Modelos de Iluminacao\n");
    printf("========================================\n");
    printf("Camera:\n");
    printf("  Mouse (arrastar botao esquerdo) - orbitar\n");
    printf("  Roda do mouse ou +/-             - zoom\n");
    printf("  W A S D                          - mover (pan)\n");
    printf("  R                                - resetar camera\n");
    printf("Iluminacao:\n");
    printf("  1 / 2 / 3   - Ambiente / Lambert / Blinn-Phong\n");
    printf("  Setas       - mover luz (eixos X/Z)\n");
    printf("  Page Up/Dn  - mover luz (eixo Y)\n");
    printf("  [ / ]       - intensidade da luz\n");
    printf("Materiais:\n");
    printf("  Tab   - seleciona categoria (Esferas -> Cones -> Cubos -> Planos)\n");
    printf("  , / . - shininess da categoria selecionada\n");
    printf("  4     - troca o material da categoria selecionada\n");
    printf("ESC - sair\n");
    printf("========================================\n\n");
    fflush(stdout);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Trabalho Final CG - Modelos de Iluminacao");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /*
     * Por padrão a OpenGL assume o observador infinitamente distante ao
     * calcular o vetor V do termo especular, o que é impreciso sob uma
     * câmera em perspectiva. Isso corrige o cálculo para usar a posição
     * real do observador em cada ponto.
     */
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    /*
     * GL_NORMALIZE faz a OpenGL renormalizar automaticamente as normais
     * após transformações (ex: escala não-uniforme).
     */
    glEnable(GL_NORMALIZE);

    /*
     * Cena expandida: 2 esferas, 2 cones, 1 cubo e o piso,
     * espaçados ao longo do eixo X. Os materiais agora são gerenciados por categoria.
     */
    Esfera* esferaA = new Esfera(-8.0f, 1.3f, 0.0f, 1.3f);
    cena.adicionarObjeto(esferaA);

    Cone* coneA = new Cone(-4.0f, 0.0f, 0.0f, 0.9f, 2.2f);
    cena.adicionarObjeto(coneA);

    Cubo* cuboObj = new Cubo(0.0f, 1.0f, 0.0f, 2.0f);
    cena.adicionarObjeto(cuboObj);

    Cone* coneB = new Cone(4.0f, 0.0f, 0.0f, 0.9f, 2.2f);
    cena.adicionarObjeto(coneB);

    Esfera* esferaB = new Esfera(8.0f, 1.3f, 0.0f, 1.3f);
    cena.adicionarObjeto(esferaB);

    Plano* planoObj = new Plano(0.0f, 0.0f, 0.0f, 26.0f);
    cena.adicionarObjeto(planoObj);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseBotao);
    glutMotionFunc(mouseMovimento);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);

    glutMainLoop();
    return 0;
}
