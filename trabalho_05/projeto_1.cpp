#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
// g++ projeto_1.cpp -o projeto_1 -lGL -lGLU -lglut -std=c++11 && ./projeto_1
// clique esquerdo: adicionar ponto / arrastar ponto existente
// clique direito: remover ponto mais próximo
// R: resetar todos os pontos
// G: alternar exibição do polígono de controle

struct Ponto {
    float x, y;
};

std::vector<Ponto> pontosControle;
int pontoSelecionado = -1;  // índice do ponto em arrasto (-1 = nenhum)
bool exibirPoligono = true;
int larguraJanela = 800;
int alturaJanela = 600;

const float RAIO_SELECAO = 10.0f; // raio de detecção para clique sobre ponto
const int NUM_SEGMENTOS = 300;     // resolução da curva desenhada

// algoritmo de De Casteljau: calcula B(t) para curva de grau n
Ponto deCasteljau(const std::vector<Ponto>& pts, float t) {
    std::vector<Ponto> temp = pts;
    int n = (int)temp.size();
    for (int r = 1; r < n; r++) {
        for (int i = 0; i < n - r; i++) {
            temp[i].x = (1.0f - t) * temp[i].x + t * temp[i + 1].x;
            temp[i].y = (1.0f - t) * temp[i].y + t * temp[i + 1].y;
        }
    }
    return temp[0];
}

// converte Y da janela GLUT (origem no topo) para Y OpenGL (origem na base)
float glutYparaGL(int y) {
    return (float)(alturaJanela - y);
}

// retorna índice do ponto mais próximo de (x,y) dentro do raio, ou -1
int encontrarPontoProximo(float x, float y) {
    for (int i = 0; i < (int)pontosControle.size(); i++) {
        float dx = pontosControle[i].x - x;
        float dy = pontosControle[i].y - y;
        if (dx * dx + dy * dy <= RAIO_SELECAO * RAIO_SELECAO)
            return i;
    }
    return -1;
}

void desenharTexto(float x, float y, const std::string& texto) {
    glRasterPos2f(x, y);
    for (char c : texto)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

void desenharPoligonoControle() {
    if (pontosControle.empty()) return;

    if (exibirPoligono && pontosControle.size() >= 2) {
        // linhas do polígono em cinza
        glColor3f(0.45f, 0.45f, 0.45f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& p : pontosControle)
            glVertex2f(p.x, p.y);
        glEnd();
    }

    // desenha cada ponto de controle individualmente para diferenciar o selecionado
    for (int i = 0; i < (int)pontosControle.size(); i++) {
        if (i == pontoSelecionado) {
            glColor3f(1.0f, 1.0f, 0.2f); // amarelo = selecionado/arrastando
            glPointSize(14.0f);
        } else {
            glColor3f(1.0f, 0.3f, 0.3f); // vermelho = padrão
            glPointSize(9.0f);
        }
        glBegin(GL_POINTS);
        glVertex2f(pontosControle[i].x, pontosControle[i].y);
        glEnd();
    }
}

void desenharCurvaBezier() {
    if (pontosControle.size() < 2) return;

    glColor3f(0.2f, 0.85f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= NUM_SEGMENTOS; i++) {
        float t = (float)i / NUM_SEGMENTOS;
        Ponto p = deCasteljau(pontosControle, t);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void desenharHUD() {
    glColor3f(0.75f, 0.75f, 0.75f);

    int numPontos = (int)pontosControle.size();
    int grau = numPontos - 1;

    std::string infoGrau = "Pontos: " + std::to_string(numPontos);
    if (grau == 1)       infoGrau += "  |  Grau 1 (linear)";
    else if (grau == 2)  infoGrau += "  |  Grau 2 (quadratica)";
    else if (grau == 3)  infoGrau += "  |  Grau 3 (cubica)";
    else if (grau > 3)   infoGrau += "  |  Grau " + std::to_string(grau) + " (superior)";

    desenharTexto(10.0f, (float)alturaJanela - 20.0f, infoGrau);
    desenharTexto(10.0f, 10.0f, "Esq:adicionar/arrastar  Dir:remover  R:resetar  G:poligono");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    desenharCurvaBezier();
    desenharPoligonoControle();
    desenharHUD();

    glutSwapBuffers();
}

void mouseClick(int button, int state, int x, int y) {
    float glX = (float)x;
    float glY = glutYparaGL(y);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            int vizinho = encontrarPontoProximo(glX, glY);
            if (vizinho >= 0) {
                pontoSelecionado = vizinho; // inicia arrasto sobre ponto existente
            } else {
                pontosControle.push_back({glX, glY}); // cria novo ponto
                pontoSelecionado = (int)pontosControle.size() - 1;
            }
        } else if (state == GLUT_UP) {
            pontoSelecionado = -1; // solta arrasto
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // remove o ponto clicado
        int vizinho = encontrarPontoProximo(glX, glY);
        if (vizinho >= 0) {
            pontosControle.erase(pontosControle.begin() + vizinho);
            pontoSelecionado = -1;
        }
    }

    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (pontoSelecionado < 0) return;
    // atualiza posição do ponto arrastado em tempo real
    pontosControle[pontoSelecionado].x = (float)x;
    pontosControle[pontoSelecionado].y = glutYparaGL(y);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        pontosControle.clear();
        pontoSelecionado = -1;
    }
    if (key == 'g' || key == 'G')
        exibirPoligono = !exibirPoligono;

    glutPostRedisplay();
}

void reshape(int w, int h) {
    larguraJanela = w;
    alturaJanela  = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Editor Interativo de Curvas de Bezier");

    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}