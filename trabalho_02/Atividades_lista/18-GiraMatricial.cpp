#include <GL/glut.h>
#include <math.h>
// para compilar: g++ 18-GiraMatricial.cpp -o quadrado-gira-matrix -lGL -lGLU -lglut -lm && ./quadrado-gira-matrix

// Angulo orbital: comeca no topo (90 graus) e decresce (sentido horario)
static float angulo     = 90.0f;
static float anguloGiro = 0.0f;

#define RAIO_ORBIT  8.0f
#define MEIO_LADO   1.5f
#define RAZAO_GIRO  4.0f   /* 360 graus proprios / 90 graus orbitais = 4 */

// Multiplica duas matrizes 4x4 column-major: resultado = A * B
// Usado para compor translacao e rotacao em uma unica matriz
void MultiplicarMatrizes(const float A[16], const float B[16], float R[16])
{
    for (int col = 0; col < 4; col++)
        for (int lin = 0; lin < 4; lin++) {
            R[col*4 + lin] = 0.0f;
            for (int k = 0; k < 4; k++)
                R[col*4 + lin] += A[k*4 + lin] * B[col*4 + k];
        }
}

// Matriz de translacao 4x4 column-major:
//
//   [ 1  0  0  tx ]
//   [ 0  1  0  ty ]
//   [ 0  0  1   0 ]
//   [ 0  0  0   1 ]
void MatrizTranslacao(float tx, float ty, float M[16])
{
    M[ 0]=1.0f; M[ 4]=0.0f; M[ 8]=0.0f; M[12]=tx;
    M[ 1]=0.0f; M[ 5]=1.0f; M[ 9]=0.0f; M[13]=ty;
    M[ 2]=0.0f; M[ 6]=0.0f; M[10]=1.0f; M[14]=0.0f;
    M[ 3]=0.0f; M[ 7]=0.0f; M[11]=0.0f; M[15]=1.0f;
}

// Matriz de rotacao 2D (em torno do eixo Z) column-major:
//
//   [ cos  -sen  0  0 ]
//   [ sen   cos  0  0 ]
//   [  0     0   1  0 ]
//   [  0     0   0  1 ]
void MatrizRotacao(float graus, float M[16])
{
    float rad  = graus * M_PI / 180.0f;
    float cosT = cos(rad);
    float sinT = sin(rad);

    M[ 0]= cosT; M[ 4]=-sinT; M[ 8]=0.0f; M[12]=0.0f;
    M[ 1]= sinT; M[ 5]= cosT; M[ 9]=0.0f; M[13]=0.0f;
    M[ 2]=0.0f;  M[ 6]=0.0f;  M[10]=1.0f; M[14]=0.0f;
    M[ 3]=0.0f;  M[ 7]=0.0f;  M[11]=0.0f; M[15]=1.0f;
}

// Quadrado centrado na origem local
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

// Marca o centro da janela
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    /* Posicao atual na orbita */
    float rad = angulo * M_PI / 180.0f;
    float px  = RAIO_ORBIT * cos(rad);
    float py  = RAIO_ORBIT * sin(rad);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Centro();
    Trajetoria();

    /* Monta as matrizes manualmente e combina em M = T * R
       Ordem matematica: primeiro rotaciona (R), depois translada (T)
       Isso e equivalente ao FILO do ex.17: glTranslatef -> glRotatef */
    float T[16], R[16], M[16];
    MatrizTranslacao(px, py, T);
    MatrizRotacao(anguloGiro, R);
    MultiplicarMatrizes(T, R, M);   /* M = T * R */

    glPushMatrix();
        glLoadMatrixf(M);
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
    glutCreateWindow("Exercicio 18 - Quadrado girando com operacoes matriciais");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}