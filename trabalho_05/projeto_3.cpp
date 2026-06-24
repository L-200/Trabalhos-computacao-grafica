#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdio>
// g++ projeto_3.cpp -o projeto_3 -lGL -lGLU -lglut -std=c++11 && ./projeto_3
// setas esquerda/direita: ajustar t manualmente passo a passo
// A: alternar animacao automatica   +/-: velocidade da animacao
// clique esquerdo: adicionar ponto de controle / arrastar existente
// clique direito: remover ponto de controle
// G: exibir/ocultar curva completa ao fundo   R: restaurar pontos iniciais

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Ponto {
    float x, y;
};

std::vector<Ponto> pontosControle = {
    {130.0f, 180.0f},
    {230.0f, 470.0f},
    {520.0f, 470.0f},
    {650.0f, 180.0f}
};

float t = 0.3f;               // parametro atual na curva [0, 1]
float velocidade = 0.005f;    // incremento de t por frame na animacao
bool animacaoAtiva = false;   // começa no modo manual para fins didáticos
bool sentidoInverso = false;
bool exibirCurva = true;      // curva completa ao fundo

int larguraJanela = 800;
int alturaJanela = 600;
int pontoSelecionado = -1;

const float RAIO_SELECAO = 12.0f;
const float PASSO_MANUAL = 0.01f; // passo das setas
const int NUM_SEGMENTOS = 300;

// uma cor distinta para cada nível de interpolação
const float coresNiveis[][3] = {
    {0.42f, 0.42f, 0.42f},  // nível 0: cinza   (polígono de controle)
    {1.0f,  0.45f, 0.1f},   // nível 1: laranja
    {1.0f,  0.95f, 0.1f},   // nível 2: amarelo
    {0.25f, 1.0f,  0.45f},  // nível 3: verde
    {0.45f, 0.6f,  1.0f},   // nível 4: azul
    {1.0f,  0.35f, 0.85f},  // nível 5: magenta
};
const int NUM_CORES = 6;

// De Casteljau completo: retorna todos os niveis intermediarios
// niveis[0] = pontos de controle originais
// niveis[r] = pontos do r-esimo nivel de interpolacao linear
// niveis.back() = { B(t) }
std::vector<std::vector<Ponto>> deCasteljauNiveis(const std::vector<Ponto>& pts, float s) {
    std::vector<std::vector<Ponto>> niveis;
    niveis.push_back(pts);

    std::vector<Ponto> atual = pts;
    while (atual.size() > 1) {
        std::vector<Ponto> proximo;
        for (int i = 0; i < (int)atual.size() - 1; i++) {
            // Qi(t) = (1 - t)*Pi + t*Pi+1
            proximo.push_back({
                (1.0f - s) * atual[i].x + s * atual[i + 1].x,
                (1.0f - s) * atual[i].y + s * atual[i + 1].y
            });
        }
        niveis.push_back(proximo);
        atual = proximo;
    }
    return niveis;
}

// apenas o ponto final B(t) para desenhar a curva completa
Ponto bezierPonto(float s) {
    return deCasteljauNiveis(pontosControle, s).back()[0];
}

float glutYparaGL(int y) {
    return (float)(alturaJanela - y);
}

int encontrarPontoProximo(float x, float y) {
    for (int i = 0; i < (int)pontosControle.size(); i++) {
        float dx = pontosControle[i].x - x;
        float dy = pontosControle[i].y - y;
        if (dx * dx + dy * dy <= RAIO_SELECAO * RAIO_SELECAO)
            return i;
    }
    return -1;
}

void desenharTexto(float x, float y, const char* texto) {
    glRasterPos2f(x, y);
    for (int i = 0; texto[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, texto[i]);
}

// curva completa ao fundo, discreta, para dar contexto visual
void desenharCurvaFundo() {
    if (!exibirCurva || pontosControle.size() < 2) return;
    glColor3f(0.22f, 0.42f, 0.6f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= NUM_SEGMENTOS; i++) {
        Ponto p = bezierPonto((float)i / NUM_SEGMENTOS);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

// núcleo do projeto: desenha cada nível do algoritmo com sua cor
void desenharNiveisDecasteljau() {
    if (pontosControle.size() < 2) return;

    auto niveis = deCasteljauNiveis(pontosControle, t);
    int grau = (int)niveis.size() - 1; // grau da curva = numero de niveis - 1

    for (int r = 0; r <= grau; r++) {
        const float* cor = coresNiveis[r % NUM_CORES];
        glColor3f(cor[0], cor[1], cor[2]);

        // segmentos de interpolacao do nivel r
        if (niveis[r].size() >= 2) {
            glLineWidth(r == 0 ? 1.0f : 1.8f);
            glBegin(GL_LINE_STRIP);
            for (const auto& p : niveis[r])
                glVertex2f(p.x, p.y);
            glEnd();
        }

        // pontos do nivel r, crescendo de tamanho a cada nivel
        glPointSize(8.0f + r * 2.0f);
        glBegin(GL_POINTS);
        for (const auto& p : niveis[r])
            glVertex2f(p.x, p.y);
        glEnd();
    }

    // B(t): ponto final do algoritmo, destacado em branco com halo
    Ponto bt = niveis.back()[0];
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(16.0f);
    glBegin(GL_POINTS);
    glVertex2f(bt.x, bt.y);
    glEnd();
}

void desenharLegenda() {
    if (pontosControle.size() < 2) return;

    int grau = (int)pontosControle.size() - 1;
    float yBase = (float)alturaJanela - 38.0f;

    for (int r = 0; r <= grau; r++) {
        const float* cor = coresNiveis[r % NUM_CORES];
        glColor3f(cor[0], cor[1], cor[2]);

        char buf[64];
        if (r == 0)
            sprintf(buf, "Nivel 0  - poligono de controle (%d pts)", grau + 1);
        else
            sprintf(buf, "Nivel %d  - interpolacao (%d pts)", r, grau - r + 1);
        desenharTexto(10.0f, yBase - r * 16.0f, buf);
    }

    // entrada da legenda para B(t)
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(10.0f, yBase - (grau + 1) * 16.0f, "B(t)  - ponto na curva");
}

void desenharHUD() {
    char buf[160];
    glColor3f(0.72f, 0.72f, 0.72f);

    int grau = (int)pontosControle.size() - 1;
    sprintf(buf, "t = %.3f  |  grau = %d  |  %s  |  vel = %.4f",
        t, grau, animacaoAtiva ? "ANIMANDO" : "MANUAL", velocidade);

    desenharTexto(10.0f, (float)alturaJanela - 18.0f, buf);
    desenharTexto(10.0f, 10.0f,
        "setas:passo manual  A:animar  +/-:vel  Esq:add  Dir:remover  G:curva  R:reset");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    desenharCurvaFundo();
    desenharNiveisDecasteljau();
    desenharLegenda();
    desenharHUD();

    glutSwapBuffers();
}

void atualizar(int valor) {
    if (animacaoAtiva) {
        if (!sentidoInverso) {
            t += velocidade;
            if (t >= 1.0f) { t = 1.0f; sentidoInverso = true; }
        } else {
            t -= velocidade;
            if (t <= 0.0f) { t = 0.0f; sentidoInverso = false; }
        }
        glutPostRedisplay();
    }
    glutTimerFunc(16, atualizar, 0); // ~60 fps
}

void mouseClick(int button, int state, int x, int y) {
    float glX = (float)x;
    float glY = glutYparaGL(y);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            int vizinho = encontrarPontoProximo(glX, glY);
            if (vizinho >= 0) {
                pontoSelecionado = vizinho;
            } else {
                pontosControle.push_back({glX, glY});
                pontoSelecionado = (int)pontosControle.size() - 1;
            }
        } else if (state == GLUT_UP) {
            pontoSelecionado = -1;
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // não remove abaixo de 2 pontos (mínimo para uma curva)
        int vizinho = encontrarPontoProximo(glX, glY);
        if (vizinho >= 0 && (int)pontosControle.size() > 2) {
            pontosControle.erase(pontosControle.begin() + vizinho);
            pontoSelecionado = -1;
        }
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (pontoSelecionado < 0) return;
    pontosControle[pontoSelecionado].x = (float)x;
    pontosControle[pontoSelecionado].y = glutYparaGL(y);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A')  animacaoAtiva = !animacaoAtiva;
    if (key == 'g' || key == 'G')  exibirCurva = !exibirCurva;
    if (key == '+' || key == '=')  velocidade = fminf(velocidade + 0.001f, 0.05f);
    if (key == '-')                velocidade = fmaxf(velocidade - 0.001f, 0.0005f);
    if (key == 'r' || key == 'R') {
        pontosControle.clear();
        pontosControle.push_back({130.0f, 180.0f});
        pontosControle.push_back({230.0f, 470.0f});
        pontosControle.push_back({520.0f, 470.0f});
        pontosControle.push_back({650.0f, 180.0f});
        t = 0.3f;
        sentidoInverso = false;
    }
    glutPostRedisplay();
}

// setas do teclado para controle fino de t no modo manual
void teclaEspecial(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) t = fminf(t + PASSO_MANUAL, 1.0f);
    if (key == GLUT_KEY_LEFT)  t = fmaxf(t - PASSO_MANUAL, 0.0f);
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
    glutCreateWindow("Visualizacao De Casteljau - Projeto 3");

    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(teclaEspecial);
    glutTimerFunc(16, atualizar, 0);

    glutMainLoop();
    return 0;
}
