# 🎓 Computação Gráfica — Atividades Práticas

Repositório com as atividades práticas desenvolvidas na disciplina de Computação Gráfica, implementadas em C com OpenGL + GLUT.

---

## 📂 Trabalhos

### [Trabalho 01 — Criação e Manipulação de Polígonos em OpenGL](./trabalho_01/)

Aplicação interativa que desenha um polígono regular 2D e permite modificar seus atributos em tempo real via teclado: número de vértices, posição, cor de preenchimento, cor do contorno, espessura da borda e modo de exibição.

**Principais conceitos:** `GL_POLYGON` · `GL_LINE_LOOP` · `GL_POINTS` · transformações 2D · callbacks GLUT

> Veja o [README do Trabalho 01](./trabalho_01/README.md) para instruções de compilação e detalhes completos.

---

### [Trabalho 02 — Transformações Geométricas 2D](./trabalho_02/)

Exercícios sobre transformações geométricas em OpenGL, divididos em duas partes: atividades dos slides da aula prática (quadrado, rotação direta/inversa, escala e animação do foguete) e exercícios da lista (órbita circular, giro no próprio eixo e pulso de escala, implementados tanto com transformações básicas quanto com operações matriciais manuais).

**Principais conceitos:** `glTranslatef` · `glRotatef` · `glScalef` · ordem FILO · `glPushMatrix`/`glPopMatrix` · matrizes 4×4 *column-major* · `glLoadMatrixf`

> Veja o [README do Trabalho 02](./trabalho_02/README.md) para instruções de compilação e detalhes completos.

---

### [Trabalho 03 — Visualização 3D em OpenGL](./trabalho_03/)

Implementação de uma câmera virtual interativa e visualização de objetos 3D. Inclui a configuração da matriz de projeção perspectiva, manipulação do observador em órbita utilizando coordenadas esféricas, aplicação de rotações em eixos isolados (X, Y, Z) e a criação de um menu interativo para renderização de primitivas geométricas do GLUT.

**Principais conceitos:** `gluPerspective` · `gluLookAt` · câmera em órbita · coordenadas esféricas · vetor Up · matrizes `GL_PROJECTION` e `GL_MODELVIEW`

> Veja o [README do trabalho 03](./trabalho_03/README.md) para instruções de compilação e detalhes completos.

---

### [Trabalho 04 — Curvas Paramétricas Clássicas em OpenGL](./trabalho_04/)

Lista de exercícios sobre representações de curvas (explícita, implícita e paramétrica) e técnicas de interpolação/aproximação (polinômio de Lagrange), seguida da implementação prática em OpenGL de um conjunto de curvas paramétricas clássicas: cônicas, cicloides, epi/hipocicloides, Lissajous, espiral de Arquimedes e Bézier cúbica em 2D, além de hélice circular, Lissajous e Nó de Trevo (Trefoil Knot) em 3D.

**Principais conceitos:** representação paramétrica × implícita × explícita · discretização do parâmetro `t` · polinômios de Lagrange · `GL_LINE_STRIP` · razão `R/r` em epi/hipocicloides · condição de fechamento de Lissajous · curvas espaciais (hélice, nó de trevo)

> Veja o [README do Trabalho 04](./trabalho_04/README.md) para instruções de compilação e detalhes completos.

---

### [Trabalho 05 — Curvas de Bézier Interativas em OpenGL](./trabalho_05/)

Lista de exercícios sobre polinômio cúbico de Hermite, curvas de Bézier, algoritmo de De Casteljau e B-Splines, seguida de quatro projetos práticos interativos: editor livre de curvas de Bézier de grau arbitrário, animação de objeto orientado pela tangente ao longo de uma trajetória cúbica, visualização didática passo a passo do algoritmo de De Casteljau com níveis de interpolação coloridos, e modelagem de pista fechada com múltiplos segmentos cúbicos conectados com continuidade C1.

**Principais conceitos:** polinômios de Bernstein · algoritmo de De Casteljau · continuidade C0 e C1 · espelhamento de alças · orientação pela tangente (`atan2`) · parâmetro global por segmento · `glutTimerFunc` · `glutMotionFunc`

> Veja o [README do Trabalho 05](./trabalho_05/README.md) para instruções de compilação e detalhes completos.

---

*Mais trabalhos serão adicionados ao longo da disciplina.*