#include <GL/glut.h>
#include <math.h>
// para compilar: g++ 16-QuadradoMatricial.cpp -o quadrado-matrix -lGL -lGLU -lglut -lm && ./quadrado-matrix

// Angulo orbital: comeca no topo (90 graus) e decresce (sentido horario)
static float angulo = 90.0f;

#define RAIO_ORBIT  8.0f
#define MEIO_LADO   1.5f

// Monta manualmente a matriz de translacao 4x4 (column-major, formato que o OpenGL espera em glLoadMatrixf):
//
//   [ 1  0  0  tx ]
//   [ 0  1  0  ty ]
//   [ 0  0  1   0 ]
//   [ 0  0  0   1 ]
//
// OpenGL armazena em column-major, entao o vetor de traducao
// fica nas posicoes [12], [13], [14] do array de 16 floats.

void CarregarMatrizTranslacao(float tx, float ty)
{
    float M[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,   /* coluna 0 */
        0.0f, 1.0f, 0.0f, 0.0f,   /* coluna 1 */
        0.0f, 0.0f, 1.0f, 0.0f,   /* coluna 2 */
        tx,   ty,   0.0f, 1.0f    /* coluna 3 - vetor de translacao */
    };
    glLoadMatrixf(M);
}

// Quadrado centrado na origem local - preto, alinhado aos eixos
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

    /* Trajetoria e centro usam a matriz identidade */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Centro();
    Trajetoria();

    /* Quadrado: carrega a matriz de translacao manualmente
       A orientacao nao muda pois a matriz nao contem rotacao */
    glPushMatrix();
        CarregarMatrizTranslacao(px, py);
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
    glutCreateWindow("Exercicio 16 - Quadrado com operacoes matriciais");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}