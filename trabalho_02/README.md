# Computação Gráfica — Atividade Prática 02
## Transformações Geométricas 2D (OpenGL/GLUT)

---

## Estrutura do Repositório

```
.
├── README.md
│
├── Atividades_slides/              # Exercícios da Aula Prática 02 (slides)
│   ├── 1-Quadrado.cpp             # Exercício 1 — Quadrado 4×4
│   ├── 2-Rotacao.cpp              # Exercício 2 — Rotação na ordem direta
│   ├── 3-RotacaoInversa.cpp       # Exercício 3 — Rotação na ordem inversa
│   ├── 4a-Escala2x.cpp            # Exercício 4a — Escala 2×
│   ├── 4b-Escala05x.cpp           # Exercício 4b — Escala 0.5×
│   └── 5-foguete.cpp              # Exercício 5 — Animação do foguete
│
└── Atividades_lista/              # Exercícios da Lista (cap. transformações)
    ├── 14-RotacaoPivo.cpp         # Exercício 14 — Rotação em torno de ponto de pivô
    ├── 15-QuadradoHorario.cpp     # Exercício 15 — Quadrado em órbita circular
    ├── 16-QuadradoMatricial.cpp   # Exercício 16 — Idem com operações matriciais
    ├── 17-QuadradoGira.cpp        # Exercício 17 — Quadrado gira no próprio centro
    ├── 18-GiraMatricial.cpp       # Exercício 18 — Idem com operações matriciais
    ├── 19-QuadradoPulsante.cpp    # Exercício 19 — Quadrado com pulso
    └── 20-PulsaMatricial.cpp      # Exercício 20 — Idem com operações matriciais
```

---

## Parte 1 — Exercícios dos Slides (Aula Prática 02)

### Exercício 1 — Quadrado 4×4
Desenho estático de um quadrado azul de lado 4 posicionado no espaço `0 ≤ x, y ≤ 10`.  
Vértices: `(3,1)`, `(7,1)`, `(7,5)`, `(3,5)`.

```bash
g++ 1-Quadrado.cpp -o quadrado -lGL -lGLU -lglut && ./quadrado
```

---

### Exercício 2 — Rotação na Ordem Direta
Aplica `glRotatef` sem compensar a posição do quadrado, fazendo-o girar em torno da **origem** (canto inferior esquerdo da janela) em vez do seu próprio centro.

```bash
g++ 2-Rotacao.cpp -o rotacao -lGL -lGLU -lglut && ./rotacao
```

**Resultado:** o quadrado aparece rotacionado em torno da origem — ele muda de posição na tela além de girar.

---

### Exercício 3 — Rotação na Ordem Inversa
Aplica a sequência correta de transformações para girar o quadrado em torno do seu **próprio centro** (ponto `(5, 3)`):

1. `glTranslatef(-5, -3, 0)` — leva o centro do quadrado à origem  
2. `glRotatef(20, 0, 0, 1)` — rotaciona 20° no eixo Z  
3. `glTranslatef(5, 3, 0)` — devolve à posição original  

> **FILO (First In, Last Out):** o OpenGL aplica as transformações na ordem **inversa** do código — os vértices passam primeiro pela translação para a origem, depois pela rotação, depois pela translação de volta.

```bash
g++ 3-RotacaoInversa.cpp -o rotacaoinversa -lGL -lGLU -lglut && ./rotacaoinversa
```

**Resultado:** o quadrado gira 20° no próprio centro sem deslocar-se.

---

### Exercício 4 — Escala 2× e 0.5×
Demonstra o efeito de `glScalef` no quadrado:

| Arquivo | Chamada | Efeito |
|---|---|---|
| `4a-Escala2x.cpp` | `glScalef(2.0, 2.0, 1.0)` | Quadrado duplica de tamanho |
| `4b-Escala05x.cpp` | `glScalef(0.5, 0.5, 1.0)` | Quadrado reduz à metade |

> A escala é aplicada em relação à **origem** do sistema de coordenadas, então o objeto também se desloca proporcionalmente se não estiver centrado na origem.

```bash
g++ 4a-Escala2x.cpp  -o escala2x  -lGL -lGLU -lglut && ./escala2x
g++ 4b-Escala05x.cpp -o escala05x -lGL -lGLU -lglut && ./escala05x
```

---

### Exercício 5 — Animação do Foguete
O foguete (bico verde, corpo azul, asas vermelhas) percorre um **semicírculo de raio 25** da esquerda para a direita, sempre com o bico apontado na direção tangencial do movimento.

**Lógica de animação:**
- O ângulo orbital começa em 180° (extremo esquerdo) e decrementa 1° a cada frame
- A posição é calculada com `px = 25·cos(θ)`, `py = 25·sin(θ)`
- A rotação tangencial é `θ − 180°`, mantendo o bico perpendicular à trajetória
- O foguete é centralizado na origem antes de ser rotacionado e transladado (`glTranslatef(-4, -4.5, 0)`)

```bash
g++ 5-foguete.cpp -o foguete -lGL -lGLU -lglut -lm && ./foguete
```

---

## Parte 2 — Exercícios da Lista

### Exercício 14 — Rotação em Torno de Ponto de Pivô
Um polígono é girado repetidamente em pequenos passos em torno de um **ponto de pivô** arbitrário no plano XY. Aproximações para seno e cosseno são usadas para acelerar os cálculos, e as coordenadas originais são redefinidas a cada nova revolução completa para evitar acúmulo de erros de arredondamento.

```bash
g++ 14-RotacaoPivo.cpp -o rotacao-pivo -lGL -lGLU -lglut -lm && ./rotacao-pivo
```

---

### Exercício 15 — Quadrado em Órbita Circular (transformações básicas)
Um quadrado preto orbita o centro da janela no **sentido horário**, como o ponteiro dos minutos de um relógio. A orientação do quadrado **não muda** durante o percurso — apenas sua posição. Implementado com transformações geométricas básicas do OpenGL.

```bash
g++ 15-QuadradoHorario.cpp -o quadrado-horario -lGL -lGLU -lglut -lm && ./quadrado-horario
```

---

### Exercício 16 — Quadrado em Órbita Circular (operações matriciais)
Mesmo comportamento do Exercício 15, porém a transformação é montada manualmente com matrizes 4×4 *column-major* e carregada via `glLoadMatrixf`.

```bash
g++ 16-QuadradoMatricial.cpp -o quadrado-matrix -lGL -lGLU -lglut -lm && ./quadrado-matrix
```

---

### Exercício 17 — Quadrado Girando no Próprio Centro (transformações básicas)
Além de orbitar, o quadrado **gira em torno do seu próprio centro** no sentido horário, completando **1 revolução própria a cada 1/4 do caminho orbital** (razão de 4° de giro próprio por 1° orbital).

**Transformações (FILO):**
```
glTranslatef(px, py, 0)        ← 2º aplicado: move para a posição na órbita
glRotatef(anguloGiro, 0, 0, 1) ← 1º aplicado: gira em torno do centro local
```

**Parâmetros:**
- Raio orbital: `8.0` unidades
- Meio-lado do quadrado: `1.5` unidades
- Razão de giro: `4°` de rotação própria por `1°` orbital
- FPS: ~60 (timer de 16 ms) — ESC encerra

```bash
g++ 17-QuadradoGira.cpp -o quadrado-gira -lGL -lGLU -lglut -lm && ./quadrado-gira
```

---

### Exercício 18 — Quadrado Girando no Próprio Centro (operações matriciais)
Mesmo comportamento do Exercício 17, com a transformação composta montada manualmente:

```
M = T × R
```

Carregada via `glLoadMatrixf(M)`.

```bash
g++ 18-GiraMatricial.cpp -o quadrado-gira-matrix -lGL -lGLU -lglut -lm && ./quadrado-gira-matrix
```

---

### Exercício 19 — Quadrado Pulsante (transformações básicas)
Além de orbitar e girar, o quadrado **pulsa**: a cada revolução completa em torno do próprio eixo, seu tamanho varia suavemente de **100% a 50%** do tamanho original. Implementado com `glScalef`.

```bash
g++ 19-QuadradoPulsante.cpp -o quadrado-pulsa -lGL -lGLU -lglut -lm && ./quadrado-pulsa
```

---

### Exercício 20 — Quadrado Pulsante (operações matriciais)
Mesmo comportamento do Exercício 19, com toda a transformação composta montada manualmente:

```
M = T × R × S
```

| Matriz | Operação |
|---|---|
| `S` | Escala uniforme — fator de pulso entre `1.0` e `0.5` |
| `R` | Rotação em torno do centro local (`anguloGiro`) |
| `T` | Translação para a posição na órbita `(px, py)` |

**Fator de escala por fase do ciclo:**
```
t = fmod(|anguloGiro|, 360°) / 360°
escala = 1.0 - 0.5 × t
```

```bash
g++ 20-PulsaMatricial.cpp -o quadrado-pulsa-matrix -lGL -lGLU -lglut -lm && ./quadrado-pulsa-matrix
```

---

## Conceitos-Chave

### Ordem FILO das Transformações
O OpenGL armazena transformações em uma matriz acumulada. As chamadas no código são aplicadas aos vértices na **ordem inversa** (FILO — First In, Last Out). Para girar um objeto em torno do seu próprio centro:

```cpp
glTranslatef(cx, cy, 0);     // 3º aplicado: volta ao lugar
glRotatef(angulo, 0, 0, 1);  // 2º aplicado: rotaciona na origem
glTranslatef(-cx, -cy, 0);   // 1º aplicado: leva o centro à origem
```

### Pilha de Matrizes
`glPushMatrix()` e `glPopMatrix()` salvam e restauram o estado da matriz corrente, evitando o acúmulo de transformações entre frames ou entre objetos distintos numa mesma cena.

### Matriz Column-Major (OpenGL)
O OpenGL usa matrizes 4×4 em formato *column-major* (colunas contíguas na memória). Um array `float M[16]` é indexado como `M[coluna*4 + linha]`. A multiplicação resulta em `CMv` para cada vértice `v` — ou seja, a última matriz especificada é aplicada **primeiro** ao vértice.

### Progressão dos Exercícios da Lista

```
Ex. 15 ──► Ex. 16   órbita circular         (básico → matricial)
Ex. 17 ──► Ex. 18   + giro próprio          (básico → matricial)
Ex. 19 ──► Ex. 20   + pulso de escala       (básico → matricial)
```

---

## Dependências

- **OpenGL** (`libGL`)
- **GLU** (`libGLU`)
- **GLUT** / **FreeGLUT** (`libglut`)
- **libm** (para `cos`, `sin`, `fmod`)

### Instalação no Ubuntu/Debian
```bash
sudo apt install freeglut3-dev libglu1-mesa-dev
```