#include <GL/glut.h>
// para compilar: g++ 2-Rotacao.cpp -o rotacao -lGL -lGLU -lglut && ./rotacao

void Quadrado()
{
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 1.0);       /* azul */
        glVertex3f(3.0, 1.0, 0.0);      /* vertice A - inferior esquerdo */
        glVertex3f(7.0, 1.0, 0.0);      /* vertice B - inferior direito  */
        glVertex3f(7.0, 5.0, 0.0);      /* vertice C - superior direito  */
        glVertex3f(3.0, 5.0, 0.0);      /* vertice D - superior esquerdo */
    glEnd();
}

void Display(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  /* fundo branco */
    glClear(GL_COLOR_BUFFER_BIT);

    /* 
       glRotatef e chamado ANTES de desenhar.
       Gira 20 graus no sentido anti-horario ao redor do eixo Z.
       O pivo e a origem (0,0) -- por isso parte do quadrado pode sair da tela, ja que seus vertices estao longe da origem. */
    glRotatef(20.0, 0.0, 0.0, 1.0);

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
    glutCreateWindow("Exercicio 2 - Rotacao Ordem Direta");

    Init();
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}