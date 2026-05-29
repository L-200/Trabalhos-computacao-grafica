// para compilar: g++ 2-MoverCamera.cpp -o 2-MoverCamera -lGL -lGLU -lglut && ./2-MoverCamera

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variaveis globais de visualizacao
GLfloat angle, fAspect;
GLdouble obsX, obsY, obsZ;

// Variaveis para o controle de orbita em coordenadas esfericas
GLfloat theta = 0.0f;    // Angulo azimutal (rotacao horizontal)
GLfloat phi = 21.8f;     // Angulo polar/elevacao (rotacao vertical)
GLfloat radius = 215.4f; // Distancia da camera

void CalcularPosicaoCamera(void)
{
    // Converte de graus para radianos
    GLfloat radTheta = theta * M_PI / 180.0f;
    GLfloat radPhi = phi * M_PI / 180.0f;

    // Converte coordenadas esfericas para cartesianas
    obsX = radius * cos(radPhi) * sin(radTheta);
    obsY = radius * sin(radPhi);
    obsZ = radius * cos(radPhi) * cos(radTheta);
}

void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 1.0f); 

    // Desenha o teapot em wire-frame
    glutWireTeapot(50.0f);

    glutSwapBuffers();
}

void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Atualiza a posicao antes de chamar o gluLookAt
    CalcularPosicaoCamera();
    gluLookAt(obsX, obsY, obsZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { 
        if (angle >= 10.0f) angle -= 5.0f;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { 
        if (angle <= 130.0f) angle += 5.0f;
    }
    
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

void Teclado(unsigned char key, int x, int y)
{
    // Orbita a camera ao redor do objeto
    switch (key) {
        case 'a': 
            theta -= 5.0f; 
            break; 
        case 'd': 
            theta += 5.0f; 
            break; 
        case 'w': 
            phi += 5.0f;   
            if (phi > 89.0f) phi = 89.0f; // Trava para nao inverter o Vetor Up
            break; 
        case 's': 
            phi -= 5.0f;   
            if (phi < -89.0f) phi = -89.0f; // Trava para nao inverter o Vetor Up
            break; 
        case 'q': 
            radius += 10.0f; 
            if (radius > 450.0f) radius = 450.0f; 
            break; 
        case 'e': 
            radius -= 10.0f; 
            if (radius < 60.0f) radius = 60.0f; 
            break; 
        case 27: 
            exit(0); 
            break;       
    }
    
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

void Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    angle = 45.0f;                        
    CalcularPosicaoCamera();              
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(350, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2 - Mover Camera");

    Inicializa();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(Teclado); 

    glutMainLoop();
    return 0;
}