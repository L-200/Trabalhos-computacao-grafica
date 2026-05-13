#include <GL/glut.h>
#include <math.h>
// para compilar: g++ 19-QuadradoPulsante.cpp -o quadrado-pulsa -lGL -lGLU -lglut -lm && ./quadrado-pulsa

// Angulo orbital: comeca no topo (90 graus) e decresce (sentido horario)
static float angulo     = 90.0f;
static float anguloGiro = 0.0f;

#define RAIO_ORBIT  8.0f
#define MEIO_LADO   1.5f
#define RAZAO_GIRO  4.0f   /* 360 graus proprios / 90 graus orbitais = 4 */

/* Calcula o fator de escala atual baseado no angulo de giro
 proprio do quadrado.

 O pulso deve:
   - comecar em 1.0 (tamanho original) quando anguloGiro = 0
   - chegar em 0.5 (metade) ao completar 360 graus de giro
   - voltar a 1.0 no inicio do proximo ciclo
 Usar interpolacao linear do angulo normalizado [0, 1]:
   t = |anguloGiro| / 360
   escala = 1.0 - 0.5 * t   ->  vai de 1.0 ate 0.5
 O modulo e usado porque anguloGiro e decrementado (horario). */

float FatorEscala()
{
    float t = fmod(fabs(anguloGiro), 360.0f) / 360.0f;   /* [0, 1) */
    return 1.0f - 0.5f * t;                               /* [1.0, 0.5] */
}

// Quadrado centrado na origem local
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

    float escala = FatorEscala();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Centro();
    Trajetoria();

    glPushMatrix();
        /* FILO: executado de baixo para cima
           1. glScalef    - aplica o pulso em torno do centro local
           2. glRotatef   - gira o quadrado em torno do seu proprio centro
           3. glTranslatef - move para a posicao na orbita               */
        glTranslatef(px, py, 0.0f);
        glRotatef(anguloGiro, 0.0f, 0.0f, 1.0f);
        glScalef(escala, escala, 1.0f);
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
    glutCreateWindow("Exercicio 19 - Quadrado orbitando, girando e pulsando");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}