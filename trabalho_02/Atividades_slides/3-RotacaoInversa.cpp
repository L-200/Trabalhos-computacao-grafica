#include <GL/glut.h>
// para compilar: g++ 3-RotacaoInversa.cpp -o rotacaoinversa -lGL -lGLU -lglut && ./rotacaoinversa

void Quadrado()
{
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 1.0);       // azul
        glVertex3f(3.0, 1.0, 0.0);      // vertice A - inferior esquerdo 
        glVertex3f(7.0, 1.0, 0.0);      // vertice B - inferior direito  
        glVertex3f(7.0, 5.0, 0.0);      // vertice C - superior direito  
        glVertex3f(3.0, 5.0, 0.0);      // vertice D - superior esquerdo 
    glEnd();
}

void Display(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  /* fundo branco */
    glClear(GL_COLOR_BUFFER_BIT);

    /*
       A sequencia abaixo faz o quadrado girar no proprio centro:

       Passo 1: translada o centro do quadrado para a origem
       Passo 2: aplica a rotacao de 20 graus
       Passo 3: translada de volta para a posicao original

       Lembrar: OpenGL aplica na ordem INVERSA do codigo (FILO),
       entao os vertices passam por: translada de volta -> rotacao -> translada para origem */

    glTranslatef(5.0, 3.0, 0.0);   // passo 3: volta ao lugar original 
    glRotatef(20.0, 0.0, 0.0, 1.0);// passo 2: rotaciona 20 graus       
    glTranslatef(-5.0, -3.0, 0.0); // passo 1: leva o centro a origem   

    Quadrado();

    glFlush();
}

void Init(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 10.0, 0.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio 3 - Rotacao Ordem Inversa");

    Init();
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}