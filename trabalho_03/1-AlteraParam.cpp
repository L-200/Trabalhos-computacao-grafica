// para compilar: g++ 1-AlteraParam.cpp -o 1-AlteraParam -lGL -lGLU -lglut && ./1-AlteraParam

#include <GL/glut.h>

GLfloat fAspect;


void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define a cor do objeto
    glColor3f(0.0f, 0.0f, 1.0f);

    // Reinicia a matriz do modelo
    glLoadIdentity();

    // Posiciona a câmera
    gluLookAt(100, 80, 200,
              0, 0, 1,
              0, 1, 0);

    // Rotação em torno do eixo X
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);

    // Rotação em torno do eixo Y
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);

    // Rotação em torno do eixo Z
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

    // Desenha o objeto
    glutWireTeapot(50.0f);

    glutSwapBuffers();
}

// Define parâmetros de visualização
void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Campo de visão = 45 graus
    gluPerspective(45.0f, fAspect, 50.0f, 400.0f);

    glMatrixMode(GL_MODELVIEW);
}

// Ajuste da janela
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

// Inicialização
void Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    fAspect = 1.0f;
}

// Programa principal
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(600, 500);

    glutCreateWindow("Visualizacao 3D - Atividade 1");

    Inicializa();

    glutDisplayFunc(Desenha);

    glutReshapeFunc(AlteraTamanhoJanela);

    glutMainLoop();

    return 0;
}