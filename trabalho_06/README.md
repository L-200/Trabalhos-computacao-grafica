# 💡 Modelos de Iluminação em OpenGL

Aplicação 3D interativa em OpenGL (pipeline fixa / legacy) que implementa e compara três modelos de iluminação - Ambiente, Lambertiano e Blinn-Phong - sobre uma cena com 5 objetos geométricos e um piso, com materiais gerenciados centralmente por categorias. A cena possui uma câmera orbital controlável e luz pontual móvel.

> Trabalho Final — Computação Gráfica

---

## 📋 Descrição

O programa exibe uma cena 3D interativa demonstrando diferentes modelos de iluminação e materiais. Os objetos não possuem cópias individuais de material. Eles solicitam o material ativo da sua respectiva categoria no momento do render, garantindo que objetos da mesma categoria (ex: todas as esferas) tenham rigorosamente o mesmo comportamento sob a luz.

---

## 🚀 Compilação e Execução

### Linux (via Makefile)
Na pasta raiz (`trabalho_06`), execute:
```bash
make
./trabalhoFinal
```

### Build Manual
Opcionalmente, o build manual pode ser feito com:
```bash
g++ src/core/*.cpp src/geometria/*.cpp src/iluminacao/*.cpp src/materiais/*.cpp -I src/core -I src/geometria -I src/iluminacao -I src/materiais -o trabalhoFinal -lGL -lGLU -lglut
```

> **Dependências:** OpenGL, GLU e GLUT (ou FreeGLUT) devem estar instalados.
> 
> No Ubuntu/Debian: `sudo apt install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev build-essential`

---

## ⌨️ Controles

### Câmera (orbital, em torno da origem)
| Tecla | Ação |
|---|---|
| Arrastar com o botão esquerdo do mouse | Orbitar |
| Roda do mouse ou `+` / `-` | Zoom |
| `W` `A` `S` `D` | Mover (pan) relativo à direção da câmera |
| `R` | Resetar câmera para a posição inicial |

### Iluminação
| Tecla | Ação |
|---|---|
| `1` | Modo: apenas iluminação Ambiente |
| `2` | Modo: Lambertiano (ambiente + difuso) |
| `3` | Modo: Blinn-Phong (ambiente + difuso + especular) |
| Setas | Mover a luz pontual (eixos X/Z) |
| `Page Up` / `Page Down` | Mover a luz pontual (eixo Y) |
| `[` / `]` | Diminuir / aumentar intensidade da luz |

> A posição da luz é marcada por uma pequena esfera amarela na cena (não reage à iluminação - é só um indicador visual).

### Materiais
| Tecla | Ação |
|---|---|
| `Tab` | Selecionar próxima categoria (Esferas → Cones → Cubos → Planos) |
| `,` / `.` | Diminuir / aumentar shininess da categoria selecionada |
| `4` | Troca sequencialmente o material da categoria selecionada |

> `ESC` encerra o programa.

---

## 📁 Estrutura do Projeto

O código-fonte em `src` está dividido em domínios lógicos:

```text
src/
├── core/
│   ├── main.cpp        // loop principal, callbacks GLUT, estado global da UI
│   ├── camera.h/cpp    // câmera orbital (coordenadas esféricas)
│   └── cena.h/cpp      // agrega e desenha os objetos na cena
├── geometria/
│   ├── objeto.h/cpp    // classe base: posição + categoria (template method desenhar)
│   ├── esfera.h/cpp    // malha UV paramétrica, normal por vértice
│   ├── cubo.h/cpp      // 6 faces subdivididas em malha, normal constante por face
│   ├── cone.h/cpp      // lateral + base subdivididas, normal constante por geratriz
│   └── plano.h/cpp     // chão subdividido em malha, normal constante
├── iluminacao/
│   └── luz.h/cpp       // luz pontual móvel (GL_LIGHT0), enum ModoIluminacao
└── materiais/
    ├── material.h/cpp              // encapsula coeficientes de cor/brilho
    └── gerenciador_materiais.h/cpp // armazena presets e controla o material ativo por categoria
```

---

## 🧠 Arquitetura e Decisões de Projeto

- **Gerenciador de Materiais Centralizado (Refatoração)**: Objetos não possuem cópias individuais de material. Eles solicitam o material ativo da sua respectiva categoria no momento do render, garantindo que objetos da mesma categoria (ex: todas as esferas) tenham rigorosamente o mesmo comportamento sob a luz.
- **Pipeline fixa (legacy), não GLSL**: escolhida por consistência com os demais trabalhos da disciplina e por não haver exigência explícita de shaders programáveis no enunciado.
- **Malhas subdivididas em vez de quads/leques únicos** (cubo, plano, cone): a pipeline fixa calcula iluminação por vértice (Gouraud shading), não por pixel. Superfícies grandes com poucos vértices "perdem" highlights especulares concentrados que caem entre os vértices existentes - subdividir aumenta a densidade de amostragem o suficiente para capturá-los corretamente.
- **`GL_LIGHT_MODEL_LOCAL_VIEWER` habilitado**: corrige o cálculo do vetor de visão (V) do termo especular para usar a posição real da câmera em vez de assumi-la infinitamente distante - relevante por usarmos projeção em perspectiva (`gluPerspective`), não ortográfica.

---

## ✅ Requisitos do Enunciado

| # | Requisito | Onde |
|---|---|---|
| 1 | Cena com ≥3 objetos | `cena.h/cpp`, 5 objetos + piso em `main.cpp` |
| 2 | Câmera controlável | `camera.h/cpp` |
| 3 | Luz pontual móvel | `luz.h/cpp`, setas/PageUp/PageDown, marcador visual |
| 4 | ≥3 materiais diferentes | `gerenciador_materiais.h/cpp` (4 presets disponíveis) |
| 5 | Iluminação ambiente | `ModoIluminacao::AMBIENTE` |
| 6 | Modelo Lambertiano | `ModoIluminacao::LAMBERT` |
| 7 | Modelo Blinn-Phong | `ModoIluminacao::BLINN_PHONG` |
| 8 | Interface teclado/mouse | callbacks em `main.cpp` |
| 9 | Exibição comparativa | troca de modo em tempo real (teclas `1`/`2`/`3`) |
| 10 | Relatório técnico | entregue separadamente |
