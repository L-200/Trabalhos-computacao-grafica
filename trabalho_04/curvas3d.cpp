#include <GL/glut.h>
#include <cmath>
// g++ curvas3d.cpp -o curvas3d -lGL -lGLU -lglut && ./curvas3d
// se mova com w,s,q,e para rotacionar a câmera

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

char currentCurve = 'A';
float angleX = 20.0f, angleY = 30.0f; // Ângulos iniciais para melhor visualização 3D

void drawHelix3D(float r, float c, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = tMax / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        glVertex3f(r * cos(t), r * sin(t), c * t);
    }
    glEnd();
}

void drawLissajous3D(float A, float B, float C, float a, float b, float c, float delta_x, float delta_y, float delta_z, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = tMax / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        float x = A * sin(a * t + delta_x);
        float y = B * sin(b * t + delta_y);
        float z = C * sin(c * t + delta_z);
        glVertex3f(x, y, z);
    }
    glEnd();
}

void drawTrefoilKnot(int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = (2.0f * M_PI) / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        float x = sin(t) + 2.0f * sin(2.0f * t);
        float y = cos(t) - 2.0f * cos(2.0f * t);
        float z = -sin(3.0f * t); 
        glVertex3f(x * 0.4f, y * 0.4f, z * 0.4f); 
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posicionamento da câmera
    glTranslatef(0.0f, 0.0f, -6.0f);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

    switch(currentCurve) {
        case 'A': 
            glPushMatrix();
            glTranslatef(0.0f, -1.5f, 0.0f); // Centraliza a hélice na tela
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            drawHelix3D(1.0f, 0.15f, 10.0f * M_PI, 500); 
            glPopMatrix();
            break;
        case 'B': drawLissajous3D(1.5f, 1.5f, 1.5f, 3.0f, 2.0f, 5.0f, 0.0f, M_PI/2.0f, 0.0f, 2.0f * M_PI, 400); break;
        case 'C': drawTrefoilKnot(400); break;
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'b' || key == 'c') key -= 32; 
    
    if (key == 'A' || key == 'B' || key == 'C') {
        currentCurve = key;
    }
    
    // Controles de câmera
    if (key == 'w') angleX -= 5.0f;
    if (key == 's') angleX += 5.0f;
    // O 'a' minúsculo conflita com a seleção da curva A se não for tratado com cuidado.
    // Como convertemos 'a' para 'A' acima, usaremos 'q' e 'e' para rotacionar horizontalmente neste arquivo.
    if (key == 'q') angleY -= 5.0f;
    if (key == 'e') angleY += 5.0f;

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Curvas 3D - Pressione A (Helice), B (Lissajous), C (Trefoil)");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}