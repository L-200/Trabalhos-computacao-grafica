/*
 * Atividade Pratica: Criacao e Manipulacao de Poligonos em OpenGL
 * Linguagem: C com OpenGL + GLUT
 *
 * Compilacao (Linux):
 *   gcc poligono_opengl.c -o poligono -lGL -lGLU -lglut -lm
 *
 * Compilacao (Windows com MinGW):
 *   gcc poligono_opengl.c -o poligono.exe -lfreeglut -lopengl32 -lglu32 -lm
 *
 * Controles do teclado:
 *   N        - Aumenta numero de vertices
 *   M        - Diminui numero de vertices
 *   Setas    - Move o poligono (cima/baixo/esq/dir)
 *   C        - Muda cor de preenchimento (cicla entre cores)
 *   V        - Muda cor da borda (cicla entre cores)
 *   B        - Aumenta espessura da borda
 *   Z        - Diminui espessura da borda
 *   P        - Alterna entre preenchido e contorno
 *   ESC      - Fecha o programa
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14
#define MAX_VERTICES 50
#define RAIO 0.4f

// Variaveis globais — atributos do poligono

int   numVertices    = 5;      // Numero de vertices do poligono regular  
float posX           = 0.0f;  // Posicao horizontal (translacao)          
float posY           = 0.0f;  // Posicao vertical (translacao)            
int   modoPreenchido = 1;     // 1 = preenchido, 0 = so contorno          
float espessura      = 2.0f;  // Espessura da borda (glLineWidth)         

// Paletas de cores disponíveis {R, G, B} 
float coresPreenche[6][3] = {
    {0.2f, 0.4f, 1.0f},  // Azul      
    {0.2f, 0.8f, 0.3f},  // Verde     
    {1.0f, 0.3f, 0.3f},  // Vermelho  
    {1.0f, 0.8f, 0.1f},  // Amarelo   
    {0.8f, 0.2f, 0.9f},  // Roxo      
    {0.1f, 0.9f, 0.9f}   // Ciano     
};

float coresBorda[5][3] = {
    {0.0f, 0.0f, 0.0f},  // Preto         
    {1.0f, 1.0f, 1.0f},  // Branco        
    {1.0f, 0.5f, 0.0f},  // Laranja       
    {0.0f, 0.5f, 0.5f},  // Verde-azulado 
    {0.9f, 0.0f, 0.5f}   // Rosa          
};

int idxCorPreenche = 0;  // Indice da cor de preenchimento atual 
int idxCorBorda    = 0;  // Indice da cor de borda atual         

// Calcula e armazena os vertices do poligono regular
void calcularVertices(float vertices[][2], int n) {
    int i;
    float angulo;
     // Distribui n vertices igualmente ao redor de um circulo usando seno e cosseno. O offset de -PI/2 faz o primeiro
     //vertice apontar para cima
     
    for (i = 0; i < n; i++) {
        angulo = 2.0f * PI * i / n - PI / 2.0f;
        vertices[i][0] = RAIO * cos(angulo);
        vertices[i][1] = RAIO * sin(angulo);
    }
}

// Exibe instrucoes no titulo da janela
void atualizarTitulo(void) {
    char titulo[256];
    sprintf(titulo,
        "Poligono OpenGL | Vertices: %d | Modo: %s | Espessura: %.1f | "
        "N/M=vertices  P=modo  C/V=cor  B/Z=espessura  Setas=mover",
        numVertices,
        modoPreenchido ? "Preenchido" : "Contorno",
        espessura);
    glutSetWindowTitle(titulo);
}

// Desenha os vertices como pontos destacados (GL_POINTS)
void desenharVertices(float vertices[][2], int n) {
    int i;
    glPointSize(8.0f);
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo para destacar os vertices 
    glBegin(GL_POINTS);
        for (i = 0; i < n; i++) {
            glVertex2f(vertices[i][0], vertices[i][1]);
        }
    glEnd();
}

// Funcao de exibicao principal (callback do GLUT)
void display(void) {
    float vertices[MAX_VERTICES][2];
    int i;

    // Limpa o buffer de cor com fundo cinza escuro 
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // carrega a matriz identidade e aplica translacao 
    glLoadIdentity();
    glTranslatef(posX, posY, 0.0f);

    calcularVertices(vertices, numVertices);

    // Desenha o preenchimento
    if (modoPreenchido) {
        glColor3f(coresPreenche[idxCorPreenche][0], coresPreenche[idxCorPreenche][1],coresPreenche[idxCorPreenche][2]);
        glBegin(GL_POLYGON);
            for (i = 0; i < numVertices; i++) {
                glVertex2f(vertices[i][0], vertices[i][1]);
            }
        glEnd();
    }

    // Desenha o contorno  
    glLineWidth(espessura);
    glColor3f(coresBorda[idxCorBorda][0],coresBorda[idxCorBorda][1],coresBorda[idxCorBorda][2]);
    glBegin(GL_LINE_LOOP);
        for (i = 0; i < numVertices; i++) {
            glVertex2f(vertices[i][0], vertices[i][1]);
        }
    glEnd();

    // Destaca os vertices com pontos
    desenharVertices(vertices, numVertices);

    // Troca os buffers
    glutSwapBuffers();

    atualizarTitulo();
}

// Callback de redimensionamento da janela
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Mantem proporcao quadrada independente do tamanho da janela 
    if (w <= h)
        gluOrtho2D(-1.0, 1.0, -1.0 * h / w, 1.0 * h / w);
    else
        gluOrtho2D(-1.0 * w / h, 1.0 * w / h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Callback de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {

        case 'n':
        case 'N':
            if (numVertices < MAX_VERTICES)
                numVertices++;
            break;

        case 'm':
        case 'M':
            if (numVertices > 3)
                numVertices--;
            break;

        case 'p':
        case 'P':
            modoPreenchido = !modoPreenchido;
            break;

        case 'c':
        case 'C':
            idxCorPreenche = (idxCorPreenche + 1) % 6; // para garantir que o indice fique dentro do limite da paleta de cores de preenchimento
            break;

        case 'v':
        case 'V':
            idxCorBorda = (idxCorBorda + 1) % 5;
            break;

        case 'b':
        case 'B':
            if (espessura < 20.0f)
                espessura += 1.0f;
            break;

        case 'z':
        case 'Z':
            if (espessura > 1.0f)
                espessura -= 1.0f;
            break;

        // ESC — Fecha o programa 
        case 27:
            exit(0);
            break;
    }

    // Solicita redesenho da tela 
    glutPostRedisplay();
}

// Callback de teclas especiais (setas de direcao)
void keyboardEspecial(int key, int x, int y) {
    float passo = 0.05f; // Quantidade de deslocamento por pressionamento 

    switch (key) {
        case GLUT_KEY_UP:    posY += passo; break;  // Seta cima    
        case GLUT_KEY_DOWN:  posY -= passo; break;  // Seta baixo   
        case GLUT_KEY_LEFT:  posX -= passo; break;  // Seta esquerda 
        case GLUT_KEY_RIGHT: posX += passo; break;  // Seta direita 
    }

    glutPostRedisplay();
}

// Funcao principal
int main(int argc, char** argv) {
    // Inicializa o GLUT 
    glutInit(&argc, argv);

    // Define double buffering e modo RGB 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Tamanho e posicao inicial da janela 
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);

    // Cria a janela 
    glutCreateWindow("Poligono OpenGL");

    // Registra os callbacks 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardEspecial);

    // Inicia o loop principal do GLUT 
    glutMainLoop();

    return 0;
}