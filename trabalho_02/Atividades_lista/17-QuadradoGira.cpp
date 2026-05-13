#include <GL/glut.h>
#include <math.h>
// para compilar: g++ 17-QuadradoGira.cpp -o quadrado-gira -lGL -lGLU -lglut -lm && ./quadrado-gira

// Angulo orbital: comeca no topo (90 graus) e decresce (sentido horario)
static float angulo = 90.0f;

#define RAIO_ORBIT  8.0f
#define MEIO_LADO   1.5f

// O quadrado deve completar 1 revolucao a cada 1/4 do caminho
// O caminho total e 360 graus, entao a cada 90 graus orbitais o quadrado completa 360 graus de rotacao propria
// Razao: 360 / 90 = 4 graus de rotacao por grau orbital
// Como o sentido e horario, a rotacao propria tambem decrementa
#define RAZAO_GIRO  4.0f       /* graus de giro proprio por grau orbital */

// Angulo de rotacao propria do quadrado (independente do orbital)
static float anguloGiro = 0.0f;

// Quadrado centrado na origem local - preto
void Quadrado()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-MEIO_LADO, -MEIO_LADO);   /* inferior esquerdo */
        glVertex2f( MEIO_LADO, -MEIO_LADO);   /* inferior direito  */
        glVertex2f( MEIO_LADO,  MEIO_LADO);   /* superior direito  */
        glVertex2f(-MEIO_LADO,  MEIO_LADO);   /* superior esquerdo */
    glEnd();
}

// Trajetoria circular - guia visual cinza claro
void Trajetoria()
{
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 72; i++) {
            float a = 2.0f * M_PI * i / 72.0f;
            glVertex2f(RAIO_ORBIT * cos(a), RAIO_ORBIT * sin(a));
        }
    glEnd();
}

// Marca o centro da janela (ponto de referencia do relogio)
void Centro()
{
    glPointSize(6.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
    glEnd();
}

void Display(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   /* fundo branco */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Posicao atual na orbita */
    float rad = angulo * M_PI / 180.0f;
    float px  = RAIO_ORBIT * cos(rad);
    float py  = RAIO_ORBIT * sin(rad);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Centro();
    Trajetoria();

    glPushMatrix();
        /* FILO: executado de baixo para cima
           1. glRotatef   - gira o quadrado em torno do seu proprio centro
           2. glTranslatef - move o quadrado para a posicao na orbita       */
        glTranslatef(px, py, 0.0f);
        glRotatef(anguloGiro, 0.0f, 0.0f, 1.0f);   /* giro proprio horario */
        Quadrado();
    glPopMatrix();

    glutSwapBuffers();
}

void Timer(int valor)
{
    /* Avanca o angulo orbital no sentido horario */
    angulo -= 1.0f;
    if (angulo < 0.0f)
        angulo += 360.0f;

    /* Giro proprio: 4 graus por grau orbital, sentido horario */
    anguloGiro -= RAZAO_GIRO;
    if (anguloGiro < 0.0f)
        anguloGiro += 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);   /* ~60 fps */
}

void Init(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-12.0, 12.0, -12.0, 12.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    glutCreateWindow("Exercicio 17 - Quadrado girando no proprio centro");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}