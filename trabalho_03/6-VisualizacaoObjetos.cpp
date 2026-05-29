// para compilar: g++ 6-VisualizacaoObjetos.cpp -o 6-VisualizacaoObjetos -lGL -lGLU -lglut && ./6-VisualizacaoObjetos

/*
1. Na funcao 'Teclado', capturamos as teclas numericas de '1' a '8' e 
   atualizamos o valor de 'objetoAtual'.
2. Na funcao 'Desenha', um bloco 'switch' avalia 'objetoAtual' e executa
    exclusivamente a funcao glut correspondente (ex: glutWireCube, glutWireSphere).
 Isso permite alternar os objetos em tempo real sem precisar recompilar o codigo.
*/

 #include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14
#endif

// Variaveis globais de visualizacao
GLfloat angle, fAspect;
GLdouble obsX, obsY, obsZ;

// Variaveis para a camera (orbita)
GLfloat theta = 0.0f;    
GLfloat phi = 21.8f;     
GLfloat radius = 215.4f; 

// Variavel de estado para selecionar qual objeto desenhar (inicia com 1 - Cubo)
int objetoAtual = 1;

void CalcularPosicaoCamera(void)
{
    GLfloat radTheta = theta * M_PI / 180.0f;
    GLfloat radPhi = phi * M_PI / 180.0f;

    obsX = radius * cos(radPhi) * sin(radTheta);
    obsY = radius * sin(radPhi);
    obsZ = radius * cos(radPhi) * cos(radTheta);
}

void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 1.0f); // Cor azul para os objetos

    glPushMatrix();

    // Menu de objetos gerados pelo teclado
    switch (objetoAtual) {
        case 1:
            glutWireCube(50.0);
            break;
        case 2:
            glutWireSphere(30.0, 20, 20);
            break;
        case 3:
            // Desloca o cone um pouco para baixo no eixo Y para centraliza-lo na tela
            glTranslatef(0.0f, -25.0f, 0.0f);
            glutWireCone(30.0, 50.0, 20, 20);
            break;
        case 4:
            glutWireTorus(10.0, 30.0, 20, 20);
            break;
        case 5:
            // Objetos platonicos geralmente tem raio 1, entao aplicamos escala
            glScalef(30.0f, 30.0f, 30.0f);
            glutWireIcosahedron();
            break;
        case 6:
            glScalef(30.0f, 30.0f, 30.0f);
            glutWireOctahedron();
            break;
        case 7:
            glScalef(30.0f, 30.0f, 30.0f);
            glutWireTetrahedron();
            break;
        case 8:
            // Dependendo da implementacao GLUT, dodecahedron pode nao aceitar raio
            glScalef(15.0f, 15.0f, 15.0f); 
            glutWireDodecahedron();
            break;
    }

    glPopMatrix();

    glutSwapBuffers();
}

void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    // Selecao do objeto (Numeros 1 a 8)
    if (key >= '1' && key <= '8') {
        objetoAtual = key - '0'; // Converte char numerico para int correspondente
    }

    // Controles da camera (Orbitar)
    switch (key) {
        case 'a': theta -= 5.0f; break; 
        case 'd': theta += 5.0f; break; 
        case 'w': 
            phi += 5.0f;   
            if (phi > 89.0f) phi = 89.0f; 
            break; 
        case 's': 
            phi -= 5.0f;   
            if (phi < -89.0f) phi = -89.0f; 
            break; 
        case 'q': 
            radius += 10.0f; 
            if (radius > 450.0f) radius = 450.0f; 
            break; 
        case 'e': 
            radius -= 10.0f; 
            if (radius < 60.0f) radius = 60.0f; 
            break; 
        case 27: exit(0); break; // ESC       
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
    glutInitWindowSize(500, 500); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("6 - Primitivas GLUT");

    Inicializa();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(Teclado); 

    glutMainLoop();
    return 0;
}