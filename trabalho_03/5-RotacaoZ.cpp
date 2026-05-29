// para compilar: g++ 5-RotacaoZ.cpp -o 5-RotacaoZ -lGL -lGLU -lglut && ./5-RotacaoZ

#include <GL/glut.h>
#include <stdlib.h>

// Variaveis globais de visualizacao
GLfloat angle, fAspect; 

void Desenha(void) 
{
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(0.0f, 0.0f, 1.0f); // Azul para o bule

    // Aplica a rotacao de 45 graus no eixo Y sobre a modelview atual
    // Deve vir apos o gluLookAt, que ja foi aplicado em EspecificaParametrosVisualizacao
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f); 

    // Desenha o teapot com a cor corrente (wire-frame)
    glutWireTeapot(50.0f); 

    // Executa os comandos OpenGL (Double Buffer)
    glutSwapBuffers(); 
}

void EspecificaParametrosVisualizacao(void) 
{
    // Especifica sistema de coordenadas de projecao
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); // Inicializa sistema de coordenadas de projecao

    // Especifica a projecao perspectiva
    gluPerspective(angle, fAspect, 0.1, 500.0); 

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); // Inicializa sistema de coordenadas do modelo

    // Especifica posicao do observador e do alvo
    gluLookAt(0.0, 80.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
}

// Funcao callback chamada quando o tamanho da janela e alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h) 
{
    // Para previnir uma divisao por zero
    if (h == 0) h = 1; 

    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h); 

    // Calcula a correcao de aspecto
    fAspect = (GLfloat)w / (GLfloat)h; 

    EspecificaParametrosVisualizacao(); 
}

// Funcao callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y) 
{
    if (button == GLUT_LEFT_BUTTON) { 
        if (state == GLUT_DOWN) { // Zoom-in
            if (angle >= 10.0f) angle -= 5.0f; 
        }
    }
    if (button == GLUT_RIGHT_BUTTON) { 
        if (state == GLUT_DOWN) { // Zoom-out
            if (angle <= 130.0f) angle += 5.0f; 
        }
    }
    
    EspecificaParametrosVisualizacao(); 
    glutPostRedisplay(); 
}

void Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    angle = 45.0f;                        
}

// Programa Principal
int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(350, 300); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("5 - Rotacao Z"); 

    Inicializa(); 

    glutDisplayFunc(Desenha); 
    glutReshapeFunc(AlteraTamanhoJanela); 
    glutMouseFunc(GerenciaMouse); 

    glutMainLoop(); 
    return 0;
}