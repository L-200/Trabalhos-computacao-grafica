# Computação Gráfica — Lista de Exercícios 03

## Estrutura do Repositório

```text
.
├── README.md
├── lista03.pdf               # Enunciado completo (20 questões conceituais + 4 projetos práticos)
├── resposta_lista_03.pdf     # Respostas das questões conceituais (Q1–Q20)
├── bezier_editor.cpp         # Projeto 1 — Editor interativo de curvas de Bézier
├── projeto_2.cpp             # Projeto 2 — Animação de objeto ao longo de curva de Bézier
├── projeto_3.cpp             # Projeto 3 — Visualização passo a passo do algoritmo de De Casteljau
└── projeto_4.cpp             # Projeto 4 — Modelagem de pista fechada com múltiplos segmentos de Bézier
```

---

## Questões Conceituais (`resposta_lista_03.pdf`)

Resolução das 20 questões teóricas sobre polinômio cúbico de Hermite, curvas de Bézier, algoritmo de De Casteljau e B-Splines. Inclui a dedução da forma matricial da curva de Hermite a partir das quatro condições de contorno `P(0)=P0, P(1)=P1, P'(0)=T0, P'(1)=T1`; a verificação de que `B(0)=P0` e `B(1)=P3` via substituição direta nos polinômios de Bernstein; a demonstração das derivadas nos extremos `B'(0)=3(P1−P0)` e `B'(1)=3(P3−P2)`; a descrição completa do algoritmo de De Casteljau para graus quadrático e cúbico; e a comparação entre curvas de Bézier e B-Splines quanto a controle local, interpolação dos extremos, influência dos pontos de controle, continuidade entre segmentos e aplicação em modelagem geométrica.

---

## Projeto 1 — Editor Interativo de Curvas de Bézier (`bezier_editor.cpp`)

Aplicação interativa que permite criar, visualizar e modificar curvas de Bézier de qualquer grau por meio da manipulação direta dos pontos de controle com o mouse. A curva é calculada via **algoritmo de De Casteljau** generalizado, suportando curvas lineares, quadráticas, cúbicas e de grau superior sem nenhuma alteração de código.

| Ação | Controle |
|---|---|
| Adicionar ponto de controle | Clique esquerdo em área vazia |
| Mover ponto de controle | Clique esquerdo e arraste sobre ponto existente |
| Remover ponto de controle | Clique direito sobre ponto |
| Alternar polígono de controle | `G` |
| Resetar todos os pontos | `R` |

```bash
g++ bezier_editor.cpp -o bezier_editor -lGL -lGLU -lglut -std=c++11 && ./bezier_editor
```

> **Grau exibido no HUD:** o número de pontos e o grau atual da curva são atualizados em tempo real no topo da janela. Adicionar pontos progressivamente permite comparar o comportamento de curvas quadráticas, cúbicas e de grau superior sem reiniciar o programa.

---

## Projeto 2 — Animação ao Longo de Curva de Bézier (`projeto_2.cpp`)

Animação de um objeto (seta triangular) percorrendo uma trajetória cúbica de Bézier. O objeto é **orientado pela tangente** da curva em cada instante — calculada a partir da derivada `B'(t)` —, e o parâmetro `t` avança automaticamente via timer, com ricochete automático nos extremos `t=0` e `t=1`. Os quatro pontos de controle são arrastáveis em tempo real.

| Controle | Ação |
|---|---|
| `ESPAÇO` | Pausar / retomar animação |
| `R` | Reiniciar do início (`t = 0`) |
| `I` | Inverter sentido de percurso |
| `+` / `-` | Aumentar / diminuir velocidade |
| `G` | Alternar exibição do polígono de controle |
| Clique esquerdo + arraste | Mover ponto de controle |

```bash
g++ projeto_2.cpp -o projeto_2 -lGL -lGLU -lglut -std=c++11 && ./projeto_2
```

> **Trilha percorrida:** o trecho de `0` a `t` é desenhado em verde mais espesso, diferenciando visualmente o trajeto já coberto da trajetória total e reforçando o sentido do parâmetro.

---

## Projeto 3 — Visualização do Algoritmo de De Casteljau (`projeto_3.cpp`)

Ferramenta didática que exibe **passo a passo** o funcionamento do algoritmo de De Casteljau. Para cada valor de `t`, todos os níveis intermediários de interpolação linear são desenhados simultaneamente, cada um em uma cor distinta. O ponto final `B(t)` é destacado em branco. Suporta qualquer grau: adicionar pontos de controle eleva o grau da curva e insere novos níveis de interpolação em tempo real.

| Cor | Nível |
|---|---|
| Cinza | Nível 0 — polígono de controle original |
| Laranja | Nível 1 — primeira interpolação |
| Amarelo | Nível 2 — segunda interpolação |
| Verde | Nível 3 — terceira interpolação |
| Branco (maior) | `B(t)` — ponto final da curva |

| Controle | Ação |
|---|---|
| `←` / `→` | Ajustar `t` manualmente (passo a passo) |
| `A` | Alternar animação automática |
| `+` / `-` | Aumentar / diminuir velocidade da animação |
| `G` | Exibir / ocultar curva completa ao fundo |
| `R` | Restaurar pontos de controle iniciais |
| Clique esquerdo | Adicionar ponto / arrastar existente |
| Clique direito | Remover ponto (mínimo de 2) |

```bash
g++ projeto_3.cpp -o projeto_3 -lGL -lGLU -lglut -std=c++11 && ./projeto_3
```

> **Modo manual:** o programa inicia com a animação desligada em `t = 0.3`, permitindo observar os níveis de interpolação estáticos antes de animar — facilitando a compreensão geométrica do algoritmo.

---

## Projeto 4 — Modelagem de Pista com Curvas de Bézier (`projeto_4.cpp`)

Modelagem de uma pista fechada composta por **4 segmentos cúbicos de Bézier** conectados, com suporte a **continuidade C1** entre segmentos. Um objeto animado percorre a pista em loop contínuo, orientado pela tangente de cada segmento. Pontos de junção (vermelhos) e alças de controle (lilás) são arrastáveis independentemente; ao arrastar uma alça com C1 ativado, a alça oposta é espelhada automaticamente em torno da junção, garantindo transição suave entre segmentos.

| Tipo de ponto | Cor | Comportamento ao arrastar |
|---|---|---|
| Junção | Vermelho | Move as duas alças adjacentes junto (preserva C0) |
| Alça | Lilás | Espelha a alça oposta na junção (C1 ativo) ou move livremente (C1 desativo) |
| Selecionado | Amarelo | — |

| Controle | Ação |
|---|---|
| `ESPAÇO` | Pausar / retomar animação |
| `C` | Alternar continuidade C1 (ON / OFF) |
| `+` / `-` | Aumentar / diminuir velocidade |
| `G` | Alternar exibição do polígono de controle |
| `R` | Restaurar pista ao formato inicial |
| Clique esquerdo + arraste | Mover ponto de controle |

```bash
g++ projeto_4.cpp -o projeto_4 -lGL -lGLU -lglut -std=c++11 && ./projeto_4
```

> **Experimento sugerido:** arraste uma alça com `C1 ON`, depois desligue com `C` e arraste a mesma alça — a quina que aparece na junção ilustra diretamente a diferença entre continuidade C0 e C1.

---

## Conceitos-Chave

### Algoritmo de De Casteljau
Método geométrico para avaliar `B(t)` por interpolações lineares sucessivas: a cada nível `r`, cada ponto `Qᵢʳ(t) = (1−t)·Qᵢʳ⁻¹ + t·Qᵢ₊₁ʳ⁻¹` até restar um único ponto, que é `B(t)`. Numericamente mais estável que a avaliação direta pelos polinômios de Bernstein e geometricamente intuitivo — todos os projetos utilizam variações desta ideia central.

### Polinômios de Bernstein
Os coeficientes `B₀³(t) = (1−t)³`, `B₁³(t) = 3(1−t)²t`, `B₂³(t) = 3(1−t)t²`, `B₃³(t) = t³` são usados explicitamente nos Projetos 2 e 4, onde a curva cúbica é avaliada diretamente. A propriedade de que sua soma é sempre 1 (partição da unidade) garante que a curva fique contida no fecho convexo dos pontos de controle.

### Continuidade C0 e C1 (Projeto 4)
**C0** exige apenas que o último ponto de um segmento coincida com o primeiro do próximo — a curva não tem buracos. **C1** exige adicionalmente que as tangentes sejam iguais na junção, o que geometricamente significa que as alças de controle adjacentes à junção sejam colineares e equidistantes dela. A fórmula de espelhamento `pts[espelho] = 2·pts[junção] − pts[alça]` implementa essa condição automaticamente.

### Parâmetro Global e Segmentação (Projeto 4)
O objeto percorre a pista com um único `tGlobal ∈ [0, NUM_SEG)`. A parte inteira identifica o segmento atual e a parte fracionária é o parâmetro local `s ∈ [0, 1)` dentro daquele segmento — decomposição análoga à de sistemas de coordenadas mistas usados em animações e simulações de trajetórias.

### Orientação pela Tangente (Projetos 2 e 4)
A derivada `B'(t)` fornece o vetor tangente à curva no parâmetro `t`. O ângulo `atan2(tan.y, tan.x)` convertido para graus é passado a `glRotatef`, fazendo o objeto apontar sempre na direção do movimento — técnica fundamental em animações de personagens e simulações de veículos.

---

## Dependências

- **OpenGL** (`libGL`)
- **GLU** (`libGLU`)
- **GLUT** / **FreeGLUT** (`libglut`)
- `cmath` para funções trigonométricas (`sin`, `cos`, `atan2`, `fminf`, `fmaxf`)
- C++11 ou superior (flag `-std=c++11`) para listas de inicialização e `std::vector`

### Instalação no Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev libgl1-mesa-dev
```
