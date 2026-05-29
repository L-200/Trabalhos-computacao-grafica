# Computação Gráfica — Atividade Prática 03
## Visualização 3D (OpenGL/GLUT)

---

## Estrutura do Repositório

```text
.
├── README.md
├── 1-AlteraParam.cpp          # Atividade 1 — Parâmetros estáticos de visualização
├── 2-MoverCamera.cpp          # Atividade 2 — Câmera em órbita (coordenadas esféricas)
├── 3-RotacaoX.cpp             # Atividade 3 — Rotação de 30° no eixo X
├── 4-RotacaoY.cpp             # Atividade 4 — Rotação de 45° no eixo Y
├── 5-RotacaoZ.cpp             # Atividade 5 — Rotação de 10° no eixo Z
└── 6-VisualizacaoObjetos.cpp  # Atividade 6 — Menu interativo com 8 primitivas GLUT
├── Pratica062026.pdf          # Slides e roteiro da aula prática
── Relatorio_slide_15.pdf     # Relatório das atividades do slide 15
```

---

## Atividades dos Slides (Aula Prática 06)

### Atividade 1 — Alterar Parâmetros de Visualização
Desenho de um `glutWireTeapot` configurado com parâmetros estáticos de visualização. O código demonstra a configuração das matrizes `GL_PROJECTION` e `GL_MODELVIEW`, aplicando as rotações pedidas diretamente na matriz do modelo de forma não-interativa.

```bash
g++ 1-AlteraParam.cpp -o 1-AlteraParam -lGL -lGLU -lglut && ./1-AlteraParam
```

---

### Atividade 2 — Mover a Câmera em Torno do Objeto
Implementação de uma câmera móvel interativa. Em vez de mover o objeto, a posição do observador (`eyex, eyey, eyez`) na função `gluLookAt` é alterada em tempo real através do teclado.

**Lógica de navegação (Coordenadas Esféricas):**
- As teclas **A/D** alteram o *azimute* (gira ao redor do eixo Y).
- As teclas **W/S** alteram a *elevação* (sobe e desce, com trava aos 89° para não inverter o vetor Up).
- As coordenadas esféricas são convertidas para cartesianas a cada frame:
  `X = raio * cos(elevação) * sin(azimute)`
  `Y = raio * sin(elevação)`
  `Z = raio * cos(elevação) * cos(azimute)`

```bash
g++ 2-MoverCamera.cpp -o 2-MoverCamera -lGL -lGLU -lglut -lm && ./2-MoverCamera
```

---

### Atividades 3, 4 e 5 — Rotações em Eixos Isolados
Demonstra o efeito prático da função `glRotatef` isolada em cada eixo cartesiano sobre a malha 3D do bule de chá. 

| Arquivo | Chamada | Efeito |
|---|---|---|
| `3-RotacaoX.cpp` | `glRotatef(30.0f, 1.0f, 0.0f, 0.0f)` | Bule "tombado" 30° para frente/trás |
| `4-RotacaoY.cpp` | `glRotatef(45.0f, 0.0f, 1.0f, 0.0f)` | Bule rotacionado 45° para o lado |
| `5-RotacaoZ.cpp` | `glRotatef(10.0f, 0.0f, 0.0f, 1.0f)` | Bule "inclinado" 10° lateralmente |

> **Isolamento de Matriz:** As rotações são encapsuladas entre `glPushMatrix()` e `glPopMatrix()` para garantir que a transformação afete apenas o objeto, e não o sistema de coordenadas global da cena.

```bash
g++ 3-RotacaoX.cpp -o 3-RotacaoX -lGL -lGLU -lglut && ./3-RotacaoX
g++ 4-RotacaoY.cpp -o 4-RotacaoY -lGL -lGLU -lglut && ./4-RotacaoY
g++ 5-RotacaoZ.cpp -o 5-RotacaoZ -lGL -lGLU -lglut && ./5-RotacaoZ
```

---

### Atividade 6 — Visualização de Múltiplos Objetos (Primitivas GLUT)
Um programa unificado e interativo que permite alternar a renderização geométrica em tempo real utilizando as teclas numéricas de **1 a 8**. A câmera em órbita foi mantida para inspecionar os objetos de todos os ângulos.

**Objetos mapeados:**
1. `glutWireCube`
2. `glutWireSphere`
3. `glutWireCone`
4. `glutWireTorus`
5. `glutWireIcosahedron` (com escala ajustada)
6. `glutWireOctahedron` (com escala ajustada)
7. `glutWireTetrahedron` (com escala ajustada)
8. `glutWireDodecahedron` (com escala ajustada)

```bash
g++ 6-VisualizacaoObjetos.cpp -o 6-VisualizacaoObjetos -lGL -lGLU -lglut -lm && ./6-VisualizacaoObjetos
```

---

## Conceitos-Chave

### Viewport vs. Projeção vs. ModelView
- **Viewport (`glViewport`):** Define a área da janela do sistema operacional onde a imagem será renderizada (geralmente mapeada para `0, 0, width, height`).
- **Matriz de Projeção (`GL_PROJECTION`):** Define a "lente" da câmera virtual. Na prática 06, utilizamos a `gluPerspective` (frustum piramidal), que cria o efeito de profundidade reduzindo objetos mais distantes.
- **Matriz de Modelo-Visualização (`GL_MODELVIEW`):** Define onde os objetos estão e de onde estamos olhando. É aqui que utilizamos o `gluLookAt` (posicionar o tripé da câmera) e o `glRotatef` (girar os objetos no cenário).

### Vetor Up (`gluLookAt`)
Os três últimos parâmetros do `gluLookAt (..., 0, 1, 0)` definem o vetor *Up* (qual direção é "para cima"). Se a câmera ultrapassar os polos (90° ou -90° no eixo Y), a linha de visão fica paralela ao vetor Up, causando inversões ou erros matemáticos na matriz. Por isso a navegação em órbita exige limites na elevação polar.

---

## Dependências e Controles

- **OpenGL** (`libGL`)
- **GLU** (`libGLU`)
- **GLUT** / **FreeGLUT** (`libglut`)
- **libm** (`-lm` necessário para as funções trigonométricas da câmera em órbita)

### Instalação no Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev libgl1-mesa-dev
```

### Controles Interativos (MoverCamera e VisualizacaoObjetos)
* **Teclas W / S / A / D:** Rotacionam a câmera em torno do objeto.
* **Teclas Q / E:** Zoom-in e Zoom-out (distância do raio na órbita).
* **Mouse (Botão Esq. / Dir.):** Alteram o ângulo `fovy` da perspectiva (lente da câmera).
* **Teclas 1 a 8:** Alternam a primitiva 3D (*apenas na Atividade 6*).
* **ESC:** Encerra a aplicação.