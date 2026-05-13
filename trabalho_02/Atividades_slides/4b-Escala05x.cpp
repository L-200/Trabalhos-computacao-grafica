#include <GL/glut.h>
// para compilar: g++ 4b-Escala05x.cpp -o escala05x -lGL -lGLU -lglut && ./escala05x

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

    /* Exercicio 4b - Escala 0.5x:
       glScalef(0.5, 0.5, 1) reduz o objeto à metade em x e y.
       O pivo da escala e a origem (0,0), entao o quadrado
       alem de reduzir de tamanho tambem se aproxima da origem.
       O lado que era 4 unidades passa a ser 2 unidades. */
    glScalef(0.5, 0.5, 1.0);

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
    glutCreateWindow("Exercicio 4b - Escala 0.5x");

    Init();
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}
