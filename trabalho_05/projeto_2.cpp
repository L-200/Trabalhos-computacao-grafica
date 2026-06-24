#include <GL/glut.h>
#include <cmath>
#include <cstdio>
// g++ projeto_2.cpp -o projeto_2 -lGL -lGLU -lglut -std=c++11 && ./projeto_2
// ESPACO: pausar/retomar animacao
// R: reiniciar do inicio   I: inverter sentido
// +/-: aumentar/diminuir velocidade
// G: alternar exibicao do poligono de controle
// arraste os pontos de controle com o mouse para modificar a trajetoria

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Ponto {
    float x, y;
};

// quatro pontos de controle da curva cubica de Bezier
Ponto pontosControle[4] = {
    {100.0f, 300.0f},
    {230.0f, 530.0f},
    {570.0f, 530.0f},
    {700.0f, 300.0f}
};

float t = 0.0f;               // parametro atual na curva [0, 1]
float velocidade = 0.003f;    // incremento de t por frame
bool sentidoInverso = false;  // false = 0->1, true = 1->0
bool animacaoAtiva = true;
bool exibirPoligono = true;

int larguraJanela = 800;
int alturaJanela = 600;
int pontoSelecionado = -1;

const float RAIO_SELECAO = 12.0f;
const int NUM_SEGMENTOS = 300;

// ponto B(t) da curva cubica de Bezier
Ponto bezierCubico(float s) {
    float u = 1.0f - s;
    return {
        u*u*u * pontosControle[0].x + 3*u*u*s * pontosControle[1].x
              + 3*u*s*s * pontosControle[2].x + s*s*s * pontosControle[3].x,
        u*u*u * pontosControle[0].y + 3*u*u*s * pontosControle[1].y
              + 3*u*s*s * pontosControle[2].y + s*s*s * pontosControle[3].y
    };
}

// derivada B'(t): vetor tangente à curva no parametro s
Ponto tangenteCubico(float s) {
    float u = 1.0f - s;
    return {
        3*u*u * (pontosControle[1].x - pontosControle[0].x)
      + 6*u*s * (pontosControle[2].x - pontosControle[1].x)
      + 3*s*s * (pontosControle[3].x - pontosControle[2].x),
        3*u*u * (pontosControle[1].y - pontosControle[0].y)
      + 6*u*s * (pontosControle[2].y - pontosControle[1].y)
      + 3*s*s * (pontosControle[3].y - pontosControle[2].y)
    };
}

float glutYparaGL(int y) {
    return (float)(alturaJanela - y);
}

int encontrarPontoProximo(float x, float y) {
    for (int i = 0; i < 4; i++) {
        float dx = pontosControle[i].x - x;
        float dy = pontosControle[i].y - y;
        if (dx*dx + dy*dy <= RAIO_SELECAO * RAIO_SELECAO)
            return i;
    }
    return -1;
}

void desenharTexto(float x, float y, const char* texto) {
    glRasterPos2f(x, y);
    for (int i = 0; texto[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, texto[i]);
}

void desenharCurva() {
    // trilha da trajetória em azul claro
    glColor3f(0.25f, 0.65f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= NUM_SEGMENTOS; i++) {
        Ponto p = bezierCubico((float)i / NUM_SEGMENTOS);
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // trecho já percorrido em verde translúcido (até t atual)
    int segPercorridos = (int)(t * NUM_SEGMENTOS);
    glColor3f(0.2f, 1.0f, 0.45f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segPercorridos; i++) {
        Ponto p = bezierCubico((float)i / NUM_SEGMENTOS);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void desenharPoligonoControle() {
    if (!exibirPoligono) return;

    // linhas conectando os pontos de controle em cinza
    glColor3f(0.42f, 0.42f, 0.42f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 4; i++)
        glVertex2f(pontosControle[i].x, pontosControle[i].y);
    glEnd();

    // os pontos de controle em si
    for (int i = 0; i < 4; i++) {
        if (i == pontoSelecionado)
            glColor3f(1.0f, 1.0f, 0.2f); // amarelo = arrastando
        else
            glColor3f(1.0f, 0.35f, 0.35f); // vermelho = padrão
        glPointSize(i == pontoSelecionado ? 13.0f : 9.0f);
        glBegin(GL_POINTS);
        glVertex2f(pontosControle[i].x, pontosControle[i].y);
        glEnd();
    }
}

// seta triangular orientada pela tangente da curva no ponto atual
void desenharObjeto() {
    Ponto pos = bezierCubico(t);
    Ponto tan = tangenteCubico(t);

    // ângulo da tangente em graus para rotacionar o objeto
    float angulo = atan2f(tan.y, tan.x) * (float)(180.0 / M_PI);

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0f);
    glRotatef(angulo, 0.0f, 0.0f, 1.0f);

    // triângulo apontando no eixo +x (será rotacionado para a direção da tangente)
    glColor3f(0.15f, 1.0f, 0.5f);
    glBegin(GL_TRIANGLES);
    glVertex2f( 16.0f,  0.0f);  // ponta frontal
    glVertex2f(-10.0f,  8.0f);  // asa superior
    glVertex2f(-10.0f, -8.0f);  // asa inferior
    glEnd();

    // contorno branco para contraste com a trilha
    glColor3f(0.9f, 0.9f, 0.9f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f( 16.0f,  0.0f);
    glVertex2f(-10.0f,  8.0f);
    glVertex2f(-10.0f, -8.0f);
    glEnd();

    glPopMatrix();

    // circulo pequeno marcando a posição P0 e P3
    glColor3f(0.9f, 0.9f, 0.9f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glVertex2f(pontosControle[0].x, pontosControle[0].y);
    glVertex2f(pontosControle[3].x, pontosControle[3].y);
    glEnd();
}

void desenharHUD() {
    char buf[160];
    glColor3f(0.72f, 0.72f, 0.72f);
    sprintf(buf, "t = %.3f  |  vel = %.4f  |  %s  |  %s",
        t, velocidade,
        sentidoInverso ? "<<< reverso" : ">>> normal",
        animacaoAtiva  ? "PLAY" : "PAUSE");
    desenharTexto(10.0f, (float)alturaJanela - 20.0f, buf);
    desenharTexto(10.0f, 10.0f,
        "ESPACO:play/pause  R:reiniciar  I:inverter  +/-:velocidade  G:poligono");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    desenharCurva();
    desenharPoligonoControle();
    desenharObjeto();
    desenharHUD();

    glutSwapBuffers();
}

void atualizar(int valor) {
    if (animacaoAtiva) {
        if (!sentidoInverso) {
            t += velocidade;
            if (t >= 1.0f) { t = 1.0f; sentidoInverso = true; } // ricochete no fim
        } else {
            t -= velocidade;
            if (t <= 0.0f) { t = 0.0f; sentidoInverso = false; } // ricochete na origem
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, atualizar, 0); // ~60 fps
}

void mouseClick(int button, int state, int x, int y) {
    float glX = (float)x;
    float glY = glutYparaGL(y);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN)
            pontoSelecionado = encontrarPontoProximo(glX, glY);
        else if (state == GLUT_UP)
            pontoSelecionado = -1;
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
    if (key == ' ')               animacaoAtiva = !animacaoAtiva;
    if (key == 'r' || key == 'R') { t = 0.0f; sentidoInverso = false; }
    if (key == 'i' || key == 'I')  sentidoInverso = !sentidoInverso;
    if (key == 'g' || key == 'G')  exibirPoligono = !exibirPoligono;
    if (key == '+' || key == '=')  velocidade = fminf(velocidade + 0.001f, 0.05f);
    if (key == '-')                velocidade = fmaxf(velocidade - 0.001f, 0.0005f);
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
    glutCreateWindow("Animacao ao Longo de Curva de Bezier - Projeto 2");

    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, atualizar, 0);

    glutMainLoop();
    return 0;
}