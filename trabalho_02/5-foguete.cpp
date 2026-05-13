#include <GL/glut.h>
#include <math.h>
// para compilar: g++ 5-foguete.cpp -o foguete -lGL -lGLU -lglut -lm && ./foguete

// Mudei o angulo inicial para começar na esquerda do semicirculo
static float angulo = 180.0f;                                                 

void Bico() {
    glBegin(GL_TRIANGLES);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(3.0,  6.0, 0.0);   // base esquerda 
        glVertex3f( 4.0,  8.0, 0.0);   // ponta        
        glVertex3f( 5.0,  6.0, 0.0);   // base direita 
    glEnd();
}

void Corpo() {
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(3.0, 1.0, 0.0);   // inferior esquerdo 
        glVertex3f( 5.0, 1.0, 0.0);   // inferior direito 
        glVertex3f( 5.0,  6.0, 0.0);   // superior direito  
        glVertex3f(3.0,  6.0, 0.0);   // superior esquerdo
    glEnd();
}

void AsaEsquerda() {
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(1.5, 1.0, 0.0);   // encosta no corpo, embaixo  
        glVertex3f(3.0, 1.0, 0.0);   // ponta externa              
        glVertex3f(3.0, 3.0, 0.0);   // encosta no corpo, em cima  
    glEnd();
}

void AsaDireita() {
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f( 5.0, 1.0, 0.0);   // encosta no corpo, embaixo  
        glVertex3f( 6.5, 1.0, 0.0);   // ponta externa              
        glVertex3f( 5.0, 3.0, 0.0);   // encosta no corpo, em cima  
    glEnd();
}

void Display(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Posicao no semicirculo */
    float rad = angulo * M_PI / 180.0f;
    float px  = 25.0f * cos(rad);   /* raio = 25 */
    float py  = 25.0f * sin(rad);

    /* Rotacao tangencial:
       angulo=180 -> rotacao=0    -> bico aponta para cima   (esquerda do arco)
       angulo=90  -> rotacao=-90  -> bico aponta p/ direita  (topo do arco)
       angulo=0   -> rotacao=-180 -> bico aponta para baixo  (direita do arco) */
    float rotacao = angulo - 180.0f;

    glPushMatrix();
        /* FILO: 3->2->1 na ordem do codigo, mas executado 1->2->3
           1. centraliza o foguete na origem (translada o centro dos vertices)
           2. rotaciona no proprio eixo
           3. move para a posicao no arco                                        */
        glTranslatef(px, py, 0.0f);
        glRotatef(rotacao, 0.0f, 0.0f, 1.0f);
        glTranslatef(-4.0f, -4.5f, 0.0f); /* Centro aproximado do foguete (X=4, Y=4.5) */

        Bico();
        Corpo();
        AsaEsquerda();
        AsaDireita();
    glPopMatrix();

    glutSwapBuffers();
}

void Timer(int valor)
{
    /* Diminui o angulo para fazer a trajetoria da Esquerda para a Direita */
    angulo -= 1.0f;
    if (angulo < 0.0f)
        angulo = 180.0f;

    glutPostRedisplay();
    glutTimerFunc(20, Timer, 0);
}

void Init(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* Espaco: -35 a +35 em x, -10 a +35 em y
       O semicirculo de raio 25 cabe com folga para ver o foguete inteiro */
    gluOrtho2D(-35.0, 35.0, -10.0, 35.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio 5 - Animacao do Foguete");

    Init();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}