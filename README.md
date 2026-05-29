# 🎓 Computação Gráfica — Atividades Práticas

Repositório com as atividades práticas desenvolvidas na disciplina de Computação Gráfica, implementadas em C com OpenGL + GLUT.

---

## 📂 Trabalhos

### [Trabalho 01 — Criação e Manipulação de Polígonos em OpenGL](./trabalho_01/)

Aplicação interativa que desenha um polígono regular 2D e permite modificar seus atributos em tempo real via teclado: número de vértices, posição, cor de preenchimento, cor do contorno, espessura da borda e modo de exibição.

**Principais conceitos:** `GL_POLYGON` · `GL_LINE_LOOP` · `GL_POINTS` · transformações 2D · callbacks GLUT

> Veja o [README do Trabalho 01](./trabalho_01/README.md) para instruções de compilação e detalhes completos.

---

### [Prática 02 — Transformações Geométricas 2D](./trabalho_02/)

Exercícios sobre transformações geométricas em OpenGL, divididos em duas partes: atividades dos slides da aula prática (quadrado, rotação direta/inversa, escala e animação do foguete) e exercícios da lista (órbita circular, giro no próprio eixo e pulso de escala, implementados tanto com transformações básicas quanto com operações matriciais manuais).

**Principais conceitos:** `glTranslatef` · `glRotatef` · `glScalef` · ordem FILO · `glPushMatrix`/`glPopMatrix` · matrizes 4×4 *column-major* · `glLoadMatrixf`

> Veja o [README da Prática 02](./trabalho_02/README.md) para instruções de compilação e detalhes completos.

---

### [Prática 03 — Visualização 3D em OpenGL](./trabalho_03/)

Implementação de uma câmera virtual interativa e visualização de objetos 3D. Inclui a configuração da matriz de projeção perspectiva, manipulação do observador em órbita utilizando coordenadas esféricas, aplicação de rotações em eixos isolados (X, Y, Z) e a criação de um menu interativo para renderização de primitivas geométricas do GLUT.

**Principais conceitos:** `gluPerspective` · `gluLookAt` · câmera em órbita · coordenadas esféricas · vetor Up · matrizes `GL_PROJECTION` e `GL_MODELVIEW`

> Veja o [README da Prática 03](./trabalho_03/README.md) para instruções de compilação e detalhes completos.

---

*Mais trabalhos serão adicionados ao longo da disciplina.*