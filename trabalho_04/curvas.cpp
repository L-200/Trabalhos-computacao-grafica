#include <GL/glut.h>
#include <cmath>
// g++ curvas.cpp -o curvas -lGL -lGLU -lglut && ./curvas

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variável para controlar qual curva desenhar
char currentCurve = '1';

// GRUPO 1: Cônicas Clássicas

void drawCircumference(float r, int numSegments) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= numSegments; i++) {
        float t = 2.0f * M_PI * float(i) / float(numSegments);
        glVertex2f(r * cos(t), r * sin(t));
    }
    glEnd();
}

void drawEllipse(float a, float b, int numSegments) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= numSegments; i++) {
        float t = 2.0f * M_PI * float(i) / float(numSegments);
        glVertex2f(a * cos(t), b * sin(t));
    }
    glEnd();
}

void drawParabola(float tMin, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = (tMax - tMin) / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = tMin + i * dt;
        glVertex2f(t, t * t);
    }
    glEnd();
}

void drawHyperbola(float a, float b, float tMin, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = (tMax - tMin) / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = tMin + i * dt;
        glVertex2f(a * cosh(t), b * sinh(t));
    }
    glEnd();
}

// GRUPO 2: Família das Cicloides

void drawCycloid(float r, float tMin, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = (tMax - tMin) / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = tMin + i * dt;
        glVertex2f(r * (t - sin(t)), r * (1 - cos(t)));
    }
    glEnd();
}

void drawEpicycloid(float R, float r, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = tMax / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        float x = (R + r) * cos(t) - r * cos(((R + r) / r) * t);
        float y = (R + r) * sin(t) - r * sin(((R + r) / r) * t);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawHypocycloid(float R, float r, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = tMax / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        float x = (R - r) * cos(t) + r * cos(((R - r) / r) * t);
        float y = (R - r) * sin(t) - r * sin(((R - r) / r) * t);
        glVertex2f(x, y);
    }
    glEnd();
}

// GRUPO 3: Outras Curvas 2D (Lissajous, Espiral, Bézier)

void drawLissajous2D(float A, float B, float a, float b, float delta, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = tMax / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        glVertex2f(A * sin(a * t + delta), B * sin(b * t));
    }
    glEnd();
}

void drawArchimedeanSpiral(float a, float b, float tMax, int numSegments) {
    glBegin(GL_LINE_STRIP);
    float dt = tMax / numSegments;
    for (int i = 0; i <= numSegments; i++) {
        float t = i * dt;
        float r = a + b * t;
        glVertex2f(r * cos(t), r * sin(t));
    }
    glEnd();
}

void drawBezier() {
    // Pontos de controle
    float P0[2] = {-0.8f, -0.5f};
    float P1[2] = {-0.4f,  0.8f};
    float P2[2] = { 0.4f, -0.8f};
    float P3[2] = { 0.8f,  0.5f};

    // Desenhando o polígono de controle (linhas tracejadas ou cor diferente)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(P0[0], P0[1]);
        glVertex2f(P1[0], P1[1]);
        glVertex2f(P2[0], P2[1]);
        glVertex2f(P3[0], P3[1]);
    glEnd();

    // Desenhando a curva
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    int numSegments = 100;
    for (int i = 0; i <= numSegments; i++) {
        float t = float(i) / float(numSegments);
        float omt = 1.0f - t;
        float b0 = omt * omt * omt;
        float b1 = 3 * omt * omt * t;
        float b2 = 3 * omt * t * t;
        float b3 = t * t * t;
        
        float x = b0*P0[0] + b1*P1[0] + b2*P2[0] + b3*P3[0];
        float y = b0*P0[1] + b1*P1[1] + b2*P2[1] + b3*P3[1];
        glVertex2f(x, y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -3.0f); // Afastar câmera para visualizar o plano 2D

    glColor3f(1.0f, 1.0f, 1.0f);

    switch(currentCurve) {
        case '1': drawCircumference(1.0f, 100); break;
        case '2': drawEllipse(1.5f, 0.8f, 100); break;
        case '3': drawParabola(-1.5f, 1.5f, 100); break;
        case '4': drawHyperbola(0.5f, 0.5f, -1.5f, 1.5f, 100); break;
        case '5': 
            glPushMatrix();
            glTranslatef(-1.5f, 0.0f, 0.0f);
            drawCycloid(0.3f, 0.0f, 4.0f * M_PI, 200); 
            glPopMatrix();
            break;
        case '6': drawEpicycloid(0.6f, 0.2f, 6.0f * M_PI, 300); break;
        case '7': drawHypocycloid(0.8f, 0.2f, 2.0f * M_PI, 300); break; // Asteróide
        case '8': drawLissajous2D(1.0f, 1.0f, 3.0f, 2.0f, M_PI/2.0f, 2.0f * M_PI, 200); break;
        case '9': drawArchimedeanSpiral(0.0f, 0.05f, 6.0f * M_PI, 300); break;
        case '0': drawBezier(); break;
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    // Se for um número de 0 a 9
    if (key >= '0' && key <= '9') {
        currentCurve = key;
        glutPostRedisplay();
    }
    
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Curvas Parametricas - Pressione 0-9 para selecionar a curva");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}