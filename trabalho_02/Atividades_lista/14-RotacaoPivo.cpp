#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
// para compilar: g++ 14-RotacaoPivo.cpp -o rotacao-pivo -lGL -lGLU -lglut -lm && ./rotacao-pivo

// Angulo orbital: posicao do poligono ao longo da trajetoria circular
static float anguloOrbital = 0.0f;

// Acumulador para detectar revolucao completa (restaurar coords originais)
static float anguloAcumulado = 0.0f;
static int   revolucoes      = 0;

// Coordenadas originais e correntes dos vertices do poligono (espaco local)
#define N 5
static float origX[N], origY[N];
static float currX[N], currY[N];

// Raio do poligono, raio da orbita e passo angular por frame
#define RAIO_POLI   1.5f
#define RAIO_ORBIT  6.0f
#define PASSO       1.5f        /* graus por frame - pequeno para aprox. Taylor */

// Aproximacoes de Taylor para seno e cosseno (angulos pequenos)
//   sin(t) = aproximadamente t - t^3/6 + t^5/120
//   cos(t) = aproximadamente 1 - t^2/2 + t^4/24
// Validas para t pequeno (PASSO em radianos em torno de 0.026 rad)
float aproxSin(float rad)
{
    float r3 = rad * rad * rad;
    float r5 = r3  * rad * rad;
    return rad - r3/6.0f + r5/120.0f;
}

float aproxCos(float rad)
{
    float r2 = rad * rad;
    float r4 = r2  * r2;
    return 1.0f - r2/2.0f + r4/24.0f;
}

// Monta o poligono regular centrado na origem (espaco local)
void InitPoligono()
{
    for (int i = 0; i < N; i++) {
        float a = (2.0f * M_PI * i / N) - M_PI / 2.0f;  /* comeca no topo */
        origX[i] = RAIO_POLI * cos(a);
        origY[i] = RAIO_POLI * sin(a);
        currX[i] = origX[i];
        currY[i] = origY[i];
    }
}

// Restaura as coordenadas originais ao fim de cada revolucao. Evita acumulo de erros de arredondamento (enunciado)
void RestaurarOriginais()
{
    for (int i = 0; i < N; i++) {
        currX[i] = origX[i];
        currY[i] = origY[i];
    }
    printf("Revolucao %d completa - coordenadas restauradas.\n", revolucoes);
}

// Aplica rotacao incremental nos pontos locais do poligono
// Matriz de rotacao 2D ponto a ponto:
//   x' = x*cos(t) - y*sin(t)
//   y' = x*sin(t) + y*cos(t)
void RotacionarPoligono(float rad)
{
    float sinT = aproxSin(rad);
    float cosT = aproxCos(rad);

    for (int i = 0; i < N; i++) {
        float nx = currX[i] * cosT - currY[i] * sinT;
        float ny = currX[i] * sinT + currY[i] * cosT;
        currX[i] = nx;
        currY[i] = ny;
    }
}

// Desenha o pivo central (ponto de referencia da orbita)
void Pivo()
{
    glPointSize(8.0f);
    glColor3f(0.85f, 0.35f, 0.12f);   /* laranja */
    glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
    glEnd();
}

// Desenha a trajetoria circular (guia visual da orbita)
void Orbita()
{
    glColor3f(0.75f, 0.75f, 0.75f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 72; i++) {
            float a = 2.0f * M_PI * i / 72.0f;
            glVertex2f(RAIO_ORBIT * cos(a), RAIO_ORBIT * sin(a));
        }
    glEnd();
}

// Desenha o raio (linha do pivo ao centro do poligono)
void Raio(float cx, float cy)
{
    glColor3f(0.7f, 0.7f, 0.7f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(cx, cy);
    glEnd();
}

// Desenha o poligono na posicao orbital (cx, cy)
// Os vertices locais (currX, currY) ja carregam a rotacao
// acumulada - basta transladar para a posicao na orbita
void Poligono(float cx, float cy)
{
    /* Contorno */
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < N; i++)
            glVertex2f(cx + currX[i], cy + currY[i]);
    glEnd();

    /* Preenchimento */
    glColor4f(0.8f, 0.8f, 0.8f, 0.5f);
    glBegin(GL_POLYGON);
        for (int i = 0; i < N; i++)
            glVertex2f(cx + currX[i], cy + currY[i]);
    glEnd();

    /* Vertices */
    glPointSize(5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
        for (int i = 0; i < N; i++)
            glVertex2f(cx + currX[i], cy + currY[i]);
    glEnd();
}

void Display(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Posicao atual do poligono na orbita */
    float rad = anguloOrbital * M_PI / 180.0f;
    float cx  = RAIO_ORBIT * cos(rad);
    float cy  = RAIO_ORBIT * sin(rad);

    Pivo();
    Orbita();
    Raio(cx, cy);
    Poligono(cx, cy);

    glutSwapBuffers();
}

void Timer(int valor)
{
    float passoRad = PASSO * M_PI / 180.0f;

    /* 1. Rotacao incremental do poligono (aprox. Taylor) */
    RotacionarPoligono(passoRad);

    /* 2. Avanca o angulo orbital */
    anguloOrbital    += PASSO;
    anguloAcumulado  += PASSO;
    if (anguloOrbital >= 360.0f)
        anguloOrbital -= 360.0f;

    /* 3. Ao completar uma volta: restaura coords originais */
    if (anguloAcumulado >= 360.0f) {
        revolucoes++;
        anguloAcumulado -= 360.0f;
        RestaurarOriginais();
    }

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);   /* ~60 fps */
}

void Init(void)
{

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* Espaco: -10 a +10 em x e y - o poligono (raio 1.5) orbita em raio 6 */
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitPoligono();
}

void Keyboard(unsigned char key, int x, int y)
{
    if (key == 27)   /* ESC encerra */
        exit(0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio 14 - Poligono em orbita circular");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}