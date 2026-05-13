#include <GL/glut.h>
// para compilar:  g++ 1-Quadrado.cpp -o quadrado -lGL -lGLU -lglut ./quadrado


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

    Quadrado();

    glFlush();
}

void Init(void)
{
    /* Define o volume de visualizacao:
       coordenadas do mundo de 0 a 10 em x e y */
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
    glutCreateWindow("Quadrado 4x4");

    Init();

    glutDisplayFunc(Display);

    glutMainLoop();
    return 0;
}