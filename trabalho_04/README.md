# Computação Gráfica — Lista de Exercícios 02

## Estrutura do Repositório

```text
.
├── README.md
├── lista02.pdf              # Enunciado completo (Parte I conceitual + Parte II prática)
├── resposta_lista_02.pdf    # Respostas das questões conceituais e de interpolação/aproximação
├── curvas.cpp                # Parte II — 10 curvas paramétricas clássicas em 2D
└── curvas3d.cpp               # Parte II — Curvas paramétricas tridimensionais + Atividade Complementar
```

---

## Parte I — Questões Conceituais (`resposta_lista_02.pdf`)

Resolução das questões teóricas sobre representações explícita, implícita e paramétrica de curvas, incluindo a análise da circunferência, da cúbica `x = t² - 1, y = t³ - t` e da curva-desafio `x = cos(t), y = sin(2t)` (figura em "oito" com auto-interseção na origem). Também responde às questões sobre interpolação e aproximação, com a derivação completa dos polinômios básicos de Lagrange e do polinômio interpolador `P(x)` para os pontos `P0(0,1)`, `P1(1,3)` e `P2(2,2)`.

---

## Parte II — Curvas Paramétricas 2D (`curvas.cpp`)

Programa único e interativo que alterna entre 10 curvas clássicas pressionando as teclas numéricas de **0 a 9**. Todas as curvas são aproximadas por segmentos de reta (`GL_LINE_STRIP`) a partir da discretização do parâmetro `t`.

| Tecla | Curva | Chamada (em `display()`) |
|---|---|---|
| `1` | Circunferência | `drawCircumference(1.0f, 100)` |
| `2` | Elipse | `drawEllipse(1.5f, 0.8f, 100)` |
| `3` | Parábola | `drawParabola(-1.5f, 1.5f, 100)` |
| `4` | Hipérbole | `drawHyperbola(0.5f, 0.5f, -1.5f, 1.5f, 100)` |
| `5` | Cicloide | `drawCycloid(0.3f, 0.0f, 4.0f*M_PI, 200)` |
| `6` | Epicicloide | `drawEpicycloid(0.6f, 0.2f, 6.0f*M_PI, 300)` |
| `7` | Hipocicloide (Asteroide) | `drawHypocycloid(0.8f, 0.2f, 2.0f*M_PI, 300)` — R/r = 4 |
| `8` | Lissajous 2D | `drawLissajous2D(1.0f, 1.0f, 3.0f, 2.0f, M_PI/2.0f, 2.0f*M_PI, 200)` |
| `9` | Espiral de Arquimedes | `drawArchimedeanSpiral(0.0f, 0.05f, 6.0f*M_PI, 300)` |
| `0` | Bézier Cúbica | `drawBezier()` — 4 pontos de controle fixos + polígono desenhado em cinza |

```bash
g++ curvas.cpp -o curvas -lGL -lGLU -lglut && ./curvas
```

> **Câmera fixa:** a cena é apenas afastada (`glTranslatef(0,0,-3)`) para enquadrar o plano XY; não há controles interativos de rotação nesta atividade, já que todas as curvas são bidimensionais.

---

## Curvas Paramétricas Tridimensionais (`curvas3d.cpp`)

Implementação das curvas espaciais da seção 0.3.1 e da Atividade Complementar, com câmera orbital controlada via teclado para inspecionar a geometria 3D sob diferentes ângulos.

| Tecla | Curva | Chamada (em `display()`) |
|---|---|---|
| `A` | Hélice Circular 3D | `drawHelix3D(1.0f, 0.15f, 10.0f*M_PI, 500)` |
| `B` | Lissajous 3D | `drawLissajous3D(1.5f,1.5f,1.5f, 3.0f,2.0f,5.0f, 0.0f, M_PI/2.0f, 0.0f, 2.0f*M_PI, 400)` |
| `C` | Nó de Trevo (Trefoil Knot) | `drawTrefoilKnot(400)` |

**Controles de câmera (coordenadas angulares, sem `gluLookAt`):**
* **W / S:** Eleva / abaixa a câmera (`angleX`)
* **Q / E:** Gira a câmera horizontalmente em torno do objeto (`angleY`)

```bash
g++ curvas3d.cpp -o curvas3d -lGL -lGLU -lglut && ./curvas3d
```

> Diferente da Atividade Prática 03, estes programas não implementam tecla de saída (ESC); para encerrar, basta fechar a janela ou usar `Ctrl+C` no terminal.

---

## Conceitos-Chave

### Discretização do Parâmetro t
Como o hardware gráfico rasteriza apenas primitivas lineares, nenhuma curva é desenhada de forma analítica/contínua. Cada função `draw*()` percorre `t` em passos `dt = tMax / numSegments` e conecta os vértices resultantes com `GL_LINE_STRIP`, aproximando a curva por uma polilinha. Reduzir `dt` (aumentar `numSegments`) melhora a suavidade visual ao custo de mais vértices processados por frame.

### Razão R/r nas (Epi/Hipo)cicloides
O número de lóbulos (epicicloide) ou cúspides (hipocicloide) é determinado pela razão `R/r`. Em `curvas.cpp`, a hipocicloide usa `R = 0.8` e `r = 0.2` — razão 4 — reproduzindo o caso particular do **asteroide**, com exatamente 4 cúspides agudas.

### Curvas Fechadas em Lissajous 3D
Uma curva de Lissajous só se fecha (é periódica) quando as razões entre todas as frequências angulares `(a, b, c)` são números racionais; caso contrário, a trajetória nunca se repete exatamente.

### Isolamento de Matriz (`curvas3d.cpp`)
A hélice usa `glPushMatrix()` / `glPopMatrix()` para aplicar uma translação e uma rotação de enquadramento sem afetar a transformação de câmera (órbita) aplicada globalmente na cena — mesmo princípio de isolamento usado nas rotações de eixo isolado de práticas anteriores.

---

## Dependências

- **OpenGL** (`libGL`)
- **GLU** (`libGLU`)
- **GLUT** / **FreeGLUT** (`libglut`)
- `cmath` para as funções trigonométricas e hiperbólicas (`sin`, `cos`, `sinh`, `cosh`)

### Instalação no Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev libgl1-mesa-dev
```