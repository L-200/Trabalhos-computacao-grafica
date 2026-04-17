# 🔷 Polígono OpenGL

Aplicação interativa em C com OpenGL + GLUT para criação e manipulação de polígonos regulares no plano bidimensional.

> Atividade Prática — Computação Gráfica

---

## 📋 Descrição

O programa desenha um polígono regular na tela e permite modificar seus atributos em tempo real via teclado: número de vértices, posição, cor de preenchimento, cor do contorno, espessura da borda e modo de exibição (preenchido ou contorno).

---

## 🚀 Compilação e Execução

### Linux
```bash
gcc poligono_opengl.c -o poligono -lGL -lGLU -lglut -lm
./poligono
```

### Windows (MinGW)
```bash
gcc poligono_opengl.c -o poligono.exe -lfreeglut -lopengl32 -lglu32 -lm
poligono.exe
```

> **Dependências:** OpenGL, GLU e GLUT (ou FreeGLUT) devem estar instalados.
>
> No Ubuntu/Debian: `sudo apt install freeglut3-dev`

---

## ⌨️ Controles

| Tecla | Ação |
|-------|------|
| `N` | Aumenta o número de vértices |
| `M` | Diminui o número de vértices (mín. 3) |
| `↑ ↓ ← →` | Move o polígono na tela |
| `C` | Alterna cor de preenchimento |
| `V` | Alterna cor da borda |
| `B` | Aumenta espessura da borda |
| `Z` | Diminui espessura da borda |
| `P` | Alterna entre preenchido e contorno |
| `ESC` | Fecha o programa |

---

## ✨ Funcionalidades

- Polígono regular com número variável de vértices (3 a 50)
- 6 cores de preenchimento disponíveis
- 5 cores de borda disponíveis
- Espessura da borda ajustável (1.0 a 20.0)
- Movimentação livre pela tela
- Modo preenchido / somente contorno
- Destaque dos vértices com pontos amarelos (`GL_POINTS`)
- Título da janela atualizado em tempo real com o estado atual

---

## 🧠 Conceitos Utilizados

- Primitivas OpenGL: `GL_POLYGON`, `GL_LINE_LOOP`, `GL_POINTS`
- Cálculo de vértices com seno e cosseno (`calcularVertices`)
- Transformações geométricas: `glTranslatef`
- Projeção ortográfica 2D: `gluOrtho2D`
- Double buffering: `glutSwapBuffers`
- Callbacks do GLUT: `glutDisplayFunc`, `glutKeyboardFunc`, `glutSpecialFunc`, `glutReshapeFunc`

---

## 📁 Estrutura do Projeto

```
.
└── capturas de tela/       # Capturas de tela da aplicação rodando
└── Atividade prática.pdf   # PDF contendo o enunciado do trabalho
└── poligono                # Código compilado (para sistemas linux)
└── poligono_opengl.c       # Código-fonte principal (comentado)
└── README.md          
└── Respostas_atividade...  # Resposta das perguntas feitas no enunciado do trabalho
```

---

## 📸 Exemplos de Uso

O programa inicia com um **pentágono** (5 vértices) centralizado na tela. A partir daí, o usuário pode:

- Pressionar `N` várias vezes para aumentar os vértices e aproximar a figura de uma circunferência
- Pressionar `P` para ver apenas o contorno
- Usar as setas para mover o polígono livremente
- Pressionar `C` e `V` para combinar diferentes cores de preenchimento e borda

