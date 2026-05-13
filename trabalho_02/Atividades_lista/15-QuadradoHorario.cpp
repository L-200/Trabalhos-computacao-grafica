#include <GL/glut.h>
#include <math.h>
// para compilar: g++ 15-QuadradoHorario.cpp -o quadrado-horario -lGL -lGLU -lglut -lm && ./quadrado-horario

// Angulo orbital: comeca no topo (90 graus) e decresce (sentido horario)
static float angulo = 90.0f;

// Raio da trajetoria e metade do lado do quadrado
#define RAIO_ORBIT  8.0f
#define MEIO_LADO   1.5f

// Desenha o quadrado centrado na origem local.
// A orientacao NAO muda - sem glRotatef, o quadrado permanece
// sempre alinhado aos eixos. Apenas transladado para a orbita.

void Quadrado()
{
    glColor3f(0.0f, 0.0f, 0.0f);   /* preto */
    glBegin(GL_QUADS);
        glVertex2f(-MEIO_LADO, -MEIO_LADO);   /* inferior esquerdo */
        glVertex2f( MEIO_LADO, -MEIO_LADO);   /* inferior direito  */
        glVertex2f( MEIO_LADO,  MEIO_LADO);   /* superior direito  */
        glVertex2f(-MEIO_LADO,  MEIO_LADO);   /* superior esquerdo */
    glEnd();
}

// Trajetoria circular
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
    glColor3f(0.5f, 0.5f, 0.5f);   /* cinza medio */
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

    Centro();
    Trajetoria();

    glPushMatrix();
        /* So translacao - orientacao do quadrado nao muda (sem glRotatef) */
        glTranslatef(px, py, 0.0f);
        Quadrado();
    glPopMatrix();

    glutSwapBuffers();
}

void Timer(int valor)
{
    /* Decrementa o angulo para percorrer no sentido horario */
    angulo -= 1.0f;
    if (angulo < 0.0f)
        angulo += 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);   /* ~60 fps */
}

void Init(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* Espaco: -12 a +12 - raio 8 + meio_lado 1.5 cabem com folga */
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
    glutCreateWindow("Exercicio 15 - Quadrado em trajetoria circular");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}