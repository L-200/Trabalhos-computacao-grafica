#include <GL/glut.h>
#include <cmath>
#include <cstdio>
// g++ projeto_4.cpp -o projeto_4 -lGL -lGLU -lglut -std=c++11 && ./projeto_4
// arraste pontos vermelhos (juncoes) ou lilases (alcas) para remodelar a pista
// C: alternar continuidade C1 (espelha alca oposta ao arrastar uma alca)
// ESPACO: pausar/retomar   +/-: velocidade   G: poligono de controle   R: reset

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Ponto {
    float x, y;
};

// 4 segmentos cubicos fechados = 12 pontos unicos (segmentos compartilham juncoes)
// segmento i usa: pts[3i], pts[3i+1], pts[3i+2], pts[(3i+3) % 12]
const int NUM_SEG  = 4;
const int NUM_PTS  = NUM_SEG * 3; // 12

Ponto pts[NUM_PTS] = {
    // juncao 0      alca saindo       alca chegando
    {200.0f, 450.0f}, {300.0f, 530.0f}, {500.0f, 530.0f},  // segmento 0
    {600.0f, 450.0f}, {660.0f, 380.0f}, {660.0f, 220.0f},  // segmento 1
    {600.0f, 150.0f}, {500.0f,  70.0f}, {300.0f,  70.0f},  // segmento 2
    {200.0f, 150.0f}, {140.0f, 220.0f}, {140.0f, 380.0f}   // segmento 3
};

float tGlobal = 0.0f;        // parametro global [0, NUM_SEG): int=segmento, frac=t local
float velocidade = 0.008f;
bool animacaoAtiva = true;
bool exibirPoligono = true;
bool modoC1 = true;          // espelha alca oposta ao arrastar, garantindo C1

int larguraJanela = 800;
int alturaJanela  = 600;
int pontoSelecionado = -1;

const float RAIO_SELECAO = 13.0f;
const int   RESOLUCAO_SEG = 80;  // pontos rasterizados por segmento

// ponto B(s) do segmento seg para s in [0,1]
Ponto bezierSeg(int seg, float s) {
    int i = seg * 3;
    int j = (i + 3) % NUM_PTS; // juncao final (fecha o loop no ultimo segmento)
    float u = 1.0f - s;
    return {
        u*u*u * pts[i].x   + 3*u*u*s * pts[i+1].x + 3*u*s*s * pts[i+2].x + s*s*s * pts[j].x,
        u*u*u * pts[i].y   + 3*u*u*s * pts[i+1].y + 3*u*s*s * pts[i+2].y + s*s*s * pts[j].y
    };
}

// derivada B'(s): vetor tangente no segmento seg para s in [0,1]
Ponto tangenteSeg(int seg, float s) {
    int i = seg * 3;
    int j = (i + 3) % NUM_PTS;
    float u = 1.0f - s;
    return {
        3*u*u * (pts[i+1].x - pts[i].x)   + 6*u*s * (pts[i+2].x - pts[i+1].x) + 3*s*s * (pts[j].x - pts[i+2].x),
        3*u*u * (pts[i+1].y - pts[i].y)   + 6*u*s * (pts[i+2].y - pts[i+1].y) + 3*s*s * (pts[j].y - pts[i+2].y)
    };
}

float glutYparaGL(int y) { return (float)(alturaJanela - y); }

int encontrarPontoProximo(float x, float y) {
    for (int i = 0; i < NUM_PTS; i++) {
        float dx = pts[i].x - x;
        float dy = pts[i].y - y;
        if (dx*dx + dy*dy <= RAIO_SELECAO * RAIO_SELECAO)
            return i;
    }
    return -1;
}

// para uma alca (idx % 3 != 0), retorna o indice da juncao associada
int juncaoDeAlca(int idx) {
    return (idx % 3 == 1) ? (idx - 1 + NUM_PTS) % NUM_PTS
                          : (idx + 1) % NUM_PTS;
}

// para uma alca, retorna o indice da alca simetrica do outro lado da juncao
// formula: pts[espelho] = 2*pts[juncao] - pts[alca]  (reflexao em torno da juncao)
int alcaEspelho(int idx) {
    return (idx % 3 == 1) ? (idx - 2 + NUM_PTS) % NUM_PTS
                          : (idx + 2) % NUM_PTS;
}

void desenharTexto(float x, float y, const char* texto) {
    glRasterPos2f(x, y);
    for (int i = 0; texto[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, texto[i]);
}

void desenharPista() {
    glColor3f(0.88f, 0.76f, 0.2f); // amarelo-ouro: cor de pista
    glLineWidth(4.0f);
    glBegin(GL_LINE_STRIP);
    for (int seg = 0; seg < NUM_SEG; seg++) {
        for (int i = 0; i <= RESOLUCAO_SEG; i++) {
            Ponto p = bezierSeg(seg, (float)i / RESOLUCAO_SEG);
            glVertex2f(p.x, p.y);
        }
    }
    // fecha o loop visualmente voltando ao inicio
    Ponto p0 = bezierSeg(0, 0.0f);
    glVertex2f(p0.x, p0.y);
    glEnd();
}

void desenharPoligonoControle() {
    if (!exibirPoligono) return;

    // linhas do poligono de cada segmento em cinza escuro
    for (int seg = 0; seg < NUM_SEG; seg++) {
        int i = seg * 3;
        int j = (i + 3) % NUM_PTS;
        glColor3f(0.35f, 0.35f, 0.35f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[i].x,   pts[i].y);
        glVertex2f(pts[i+1].x, pts[i+1].y);
        glVertex2f(pts[i+2].x, pts[i+2].y);
        glVertex2f(pts[j].x,   pts[j].y);
        glEnd();
    }

    // pontos: juncoes em vermelho (maiores), alcas em lila (menores)
    for (int i = 0; i < NUM_PTS; i++) {
        bool eJuncao = (i % 3 == 0);

        if (i == pontoSelecionado)      glColor3f(1.0f, 1.0f, 0.2f);   // amarelo = selecionado
        else if (eJuncao)               glColor3f(1.0f, 0.35f, 0.35f); // vermelho = juncao
        else                            glColor3f(0.65f, 0.45f, 1.0f); // lila = alca

        glPointSize(eJuncao ? 11.0f : 7.5f);
        glBegin(GL_POINTS);
        glVertex2f(pts[i].x, pts[i].y);
        glEnd();
    }
}

void desenharObjeto() {
    // decompoe tGlobal em segmento (parte inteira) + parametro local (parte fracionaria)
    int   seg = (int)tGlobal % NUM_SEG;
    float s   = tGlobal - (int)tGlobal;

    Ponto pos    = bezierSeg(seg, s);
    Ponto tan    = tangenteSeg(seg, s);
    float angulo = atan2f(tan.y, tan.x) * (180.0f / (float)M_PI);

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0f);
    glRotatef(angulo, 0.0f, 0.0f, 1.0f);

    // triangulo apontando em +x, rotacionado para a tangente
    glColor3f(0.2f, 0.9f, 0.5f);
    glBegin(GL_TRIANGLES);
    glVertex2f( 15.0f,  0.0f);
    glVertex2f(-10.0f,  8.0f);
    glVertex2f(-10.0f, -8.0f);
    glEnd();

    glColor3f(0.92f, 0.92f, 0.92f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f( 15.0f,  0.0f);
    glVertex2f(-10.0f,  8.0f);
    glVertex2f(-10.0f, -8.0f);
    glEnd();

    glPopMatrix();
}

void desenharHUD() {
    char buf[160];
    glColor3f(0.72f, 0.72f, 0.72f);
    sprintf(buf, "seg = %d  |  t = %.3f  |  vel = %.4f  |  C1 = %s  |  %s",
        (int)tGlobal % NUM_SEG,
        tGlobal - (int)tGlobal,
        velocidade,
        modoC1 ? "ON" : "OFF",
        animacaoAtiva ? "PLAY" : "PAUSE");
    desenharTexto(10.0f, (float)alturaJanela - 18.0f, buf);
    desenharTexto(10.0f, 10.0f,
        "arraste pontos  ESPACO:play/pause  C:C1  +/-:vel  G:poligono  R:reset");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    desenharPista();
    desenharPoligonoControle();
    desenharObjeto();
    desenharHUD();

    glutSwapBuffers();
}

void atualizar(int valor) {
    if (animacaoAtiva) {
        tGlobal += velocidade;
        if (tGlobal >= (float)NUM_SEG)
            tGlobal -= (float)NUM_SEG; // loop continuo na pista fechada
        glutPostRedisplay();
    }
    glutTimerFunc(16, atualizar, 0); // ~60 fps
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN)
            pontoSelecionado = encontrarPontoProximo((float)x, glutYparaGL(y));
        else if (state == GLUT_UP)
            pontoSelecionado = -1;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (pontoSelecionado < 0) return;

    float glX = (float)x;
    float glY = glutYparaGL(y);
    float dx  = glX - pts[pontoSelecionado].x;
    float dy  = glY - pts[pontoSelecionado].y;

    pts[pontoSelecionado].x = glX;
    pts[pontoSelecionado].y = glY;

    int mod = pontoSelecionado % 3;

    if (mod == 0) {
        // juncao: arrasta as duas alcas adjacentes junto para preservar C0
        int antes  = (pontoSelecionado - 1 + NUM_PTS) % NUM_PTS;
        int depois = (pontoSelecionado + 1) % NUM_PTS;
        if (antes  % 3 != 0) { pts[antes].x  += dx; pts[antes].y  += dy; }
        if (depois % 3 != 0) { pts[depois].x += dx; pts[depois].y += dy; }
    } else if (modoC1) {
        // alca: espelha a alca oposta em relacao a juncao para garantir C1
        // pts[espelho] = 2*pts[juncao] - pts[alca_arrastada]
        int juncao  = juncaoDeAlca(pontoSelecionado);
        int espelho = alcaEspelho(pontoSelecionado);
        pts[espelho].x = 2.0f * pts[juncao].x - pts[pontoSelecionado].x;
        pts[espelho].y = 2.0f * pts[juncao].y - pts[pontoSelecionado].y;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ')               animacaoAtiva = !animacaoAtiva;
    if (key == 'c' || key == 'C') modoC1 = !modoC1;
    if (key == 'g' || key == 'G') exibirPoligono = !exibirPoligono;
    if (key == '+' || key == '=') velocidade = fminf(velocidade + 0.002f, 0.12f);
    if (key == '-')               velocidade = fmaxf(velocidade - 0.002f, 0.001f);
    if (key == 'r' || key == 'R') {
        Ponto inicial[NUM_PTS] = {
            {200.0f, 450.0f}, {300.0f, 530.0f}, {500.0f, 530.0f},
            {600.0f, 450.0f}, {660.0f, 380.0f}, {660.0f, 220.0f},
            {600.0f, 150.0f}, {500.0f,  70.0f}, {300.0f,  70.0f},
            {200.0f, 150.0f}, {140.0f, 220.0f}, {140.0f, 380.0f}
        };
        for (int i = 0; i < NUM_PTS; i++) pts[i] = inicial[i];
        tGlobal = 0.0f;
    }
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
    glutCreateWindow("Modelagem de Pista com Curvas de Bezier - Projeto 4");

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