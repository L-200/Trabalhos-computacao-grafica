# Documentação Técnica do Projeto – Modelos de Iluminação (OpenGL Legacy)

---

## 1. Visão geral

O **objetivo central** deste projeto é implementar, demonstrar e comparar os clássicos modelos de iluminação local e técnicas de sombreamento na computação gráfica 3D, especificamente utilizando a *Fixed Function Pipeline* (FFP) do OpenGL Legacy em C++.

A **proposta** é oferecer um ambiente interativo visual onde o usuário possa isolar e visualizar em tempo real as contribuições da luz ambiente, reflexão difusa (Lambert) e reflexão especular (Blinn-Phong) aplicadas sobre formas geométricas distintas.

O **problema resolvido** é a complexidade didática de compreender os vetores normais, parâmetros de luz e interações da superfície. Ao modularizar a aplicação e prover controles intuitivos, o projeto provê o entendimento prático dos coeficientes da equação de iluminação sem a necessidade de reescrever o código. A **motivação** é criar um caso de estudo estruturalmente forte, limpo e modularizado que evite os "códigos espaguete" comuns em trabalhos acadêmicos tradicionais de OpenGL.

---

## 2. Atendimento ao enunciado (Inferido a partir da estrutura)

Abaixo estão os requisitos obrigatórios extraídos da natureza da implementação do projeto:

* **[Implementado] Primitivas Geométricas:** Exibição simultânea de esferas, cones, cubos e um plano (piso). 
  * *Onde:* Diretório `src/geometria/`, arquivos `esfera.cpp`, `cone.cpp`, `cubo.cpp`, `plano.cpp`.
* **[Implementado] Modelo de Iluminação Ambiente:** Aplicação do termo básico de luminosidade uniforme que não depende de direção.
  * *Onde:* `src/iluminacao/luz.cpp`, modo `ModoIluminacao::AMBIENTE`.
* **[Implementado] Modelo Lambert (Difuso):** Cálculo que varia segundo o ângulo entre a luz e a normal.
  * *Onde:* `luz.cpp`, através da ativação condicional do parâmetro `GL_DIFFUSE`.
* **[Implementado] Modelo Blinn-Phong (Especular):** Adição do brilho refletivo direcional e cálculo da excentricidade (shininess).
  * *Onde:* `luz.cpp`, combinando `GL_DIFFUSE` com `GL_SPECULAR` e ajuste global de propriedades via `GL_SHININESS` (`material.cpp`).
* **[Implementado] Câmera Interativa:** Controle orbital com translação livre em relação ao ponto de foco, zoom e movimentação (Pan).
  * *Onde:* `src/core/camera.cpp`.
* **[Implementado] Fonte de luz interativa:** O usuário deve conseguir transladar a fonte pontual de luz pelo ambiente e notar a variação da luz.
  * *Onde:* `luz.cpp` e atalhos de teclado mapeados no `main.cpp`.
* **[Implementado] Aplicação Dinâmica de Materiais:** Configuração e alteração de propriedades físicas simuladas.
  * *Onde:* Arquivos na pasta `src/materiais/`. Foi implementado um gerenciador (`GerenciadorMateriais`) que agrupa propriedades fisicamente coesas para Plástico, Metal, Borracha e Cerâmica.
* **[Implementado] Controles Paramétricos em Tempo Real:** Ajustes finos de intensidade luminosa e de índice especular (*Shininess*).
  * *Onde:* Manipulado diretamente via eventos de teclado no `main.cpp`.

---

## 3. Arquitetura

A arquitetura do projeto adota a orientação a objetos, dividindo o código em camadas responsivas para promover a modularidade.

* **Estrutura de diretórios:**
  * `/src/core/`: Classes essenciais de infraestrutura. Guarda o `main.cpp` (ponto de entrada e gerenciador de estado da interface com GLUT), a `Cena` (agregador de formas) e a `Camera`.
  * `/src/geometria/`: Modelagem das primitivas em malhas (*meshes*) parametrizadas. Possui a classe base abstrata `Objeto` que provê propriedades universais de translação espacial.
  * `/src/iluminacao/`: A classe `Luz`, atuando como invólucro para a `GL_LIGHT0`, encapsulando os estados ativos do modelo de cor e intensidade luminosa.
  * `/src/materiais/`: A classe base de `Material` (agregando fatores RGBA para ambiente, difuso e especular, além de *shininess*) e o *Singleton* `GerenciadorMateriais`, o orquestrador global de presets e distribuidor desses materiais aos objetos na hora da renderização.

* **Fluxo Geral:**
  O sistema é impulsionado por um loop clássico assíncrono controlado pelo GLUT. Quando ocorre alguma entrada (`teclado`, `mouse`), variáveis internas sofrem mutação, chamando o redesenho (`glutPostRedisplay()`). O desenho executa a limpeza dos buffers, aplica as matrizes da câmera, projeta a luz sobre a cena e percorre todos os objetos, cada qual requisitando ao singleton seu material ativo antes de executar suas diretivas `glBegin()`/`glEnd()`.

---

## 4. Fluxo de execução

1. **Início e Inicialização (`main.cpp`):** O programa recebe os argumentos via `main()`, printa os controles no terminal, inicializa o FreeGLUT (`glutInit`) e solicita *double buffering*, buffer de profundidade e cores RGB (`GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH`). 
2. **Criação da Janela:** `glutCreateWindow` abre a janela principal e callbacks são registrados (`display`, `reshape`, mouse e teclado).
3. **Configuração Inicial do OpenGL:** Habilita-se *Depth Test* (`GL_DEPTH_TEST`), a iluminação global nativa (`GL_LIGHTING`) e a luz 0 (`GL_LIGHT0`). O modelo local viewer é habilitado para reflexões especulares matematicamente apuradas, assim como `GL_NORMALIZE` para as matrizes.
4. **Carregamento dos Objetos:** Esferas, Cones, Cubo e Plano são dinamicamente alocados na `heap` (`new`) e despachados para o array da classe `Cena`.
5. **Configuração dos Materiais e Luz:** O `GerenciadorMateriais` nasce (Lazy Initialization). A `Luz` inicial tem posição predefinida.
6. **Render Loop:** O FreeGLUT assume o processamento (`glutMainLoop`). Toda vez que `display()` é provocado, a renderização atualiza.

---

## 5. Estruturas importantes

* **`Camera` (Classe):** Representa a abstração do `gluLookAt`. Possui responsabilidade de converter suas próprias coordenadas esféricas (azimute, elevação, raio orbital) numa posição cartesiana `(x, y, z)` apontada a um `alvo`. Garante a prevenção de *Gimbal Flip* ao travar a elevação em ±89 graus.
* **`Objeto` (Classe abstrata):** Classe base de todo desenhável na cena. Força o contrato polimórfico `desenharGeometria()`. A responsabilidade é executar a translação no mundo (coordenadas fixas `posX, posY, posZ`), requisitar e aplicar o `Material` ativo correspondente à sua `CategoriaObjeto` via `glMaterialfv`.
* **`GerenciadorMateriais` (Singleton):** Atua com papel regulador da interface. Ao invés de lidar com dezenas de instâncias com parâmetros divergentes, o gerenciador concentra uma `map` ligando uma `CategoriaObjeto` a um único `Material`. Responsável por garantir coerência (todas as esferas terão a exata mesma cor e reação à luz no momento X).
* **`Luz` (Classe):** Controladora do modo estrito de iluminação (Ambiente puro, Lambert, Blinn-Phong). Ela detém valores fixos de coeficientes difusos/especulares 100% brancos, mutando a cor da emissão para preto (0,0,0) dependendo do modo isolado selecionado pelo usuário.

---

## 6. Objetos da cena

* **Esfera (2x):**
  * *Finalidade:* Demonstrar uma variação contínua e perfeita do vetor normal (a normal de cada vértice é sua própria posição num raio normalizado), ideal para exibir o sombreamento suave (*Smooth Shading*, Gouraud).
  * *Geometria:* Malha paramétrica composta de faixas de latitude formadas por `GL_TRIANGLE_STRIP`.
  * *Categoria / Material Default:* ESFERA / Plástico Vermelho.
* **Cone (2x):**
  * *Finalidade:* Formas complexas apresentando um polo divergente. Exibe as normais ortogonais às geratrizes, ou seja, constante verticalmente mas oscilante circunferencialmente. A base do cone é modelada através de anéis `GL_TRIANGLE_STRIP` ao invés de grandes triângulos `GL_TRIANGLE_FAN` unicamente conectados ao meio (evitando deficiências de especularidade no centro fixo).
  * *Categoria / Material Default:* CONE / Metal Prateado.
* **Cubo (1x):**
  * *Finalidade:* Exemplificar o comportamento sobre faces achatadas em contraste com curvas; útil para explicar *flat shading*. O brilho depende de um vetor normal puramente perpendicular ao plano.
  * *Geometria:* Seis faces construídas subdividindo malhas menores (um grid de quads) invés de triângulos imensos, forçando a interpolação de iluminação por vértice mais vezes ao longo da face para manter o cálculo especular exato.
  * *Categoria / Material Default:* CUBO / Borracha Azul.
* **Plano (1x):**
  * *Finalidade:* Base referencial imitando um piso físico ("chão") e para testar como as sombras da malha absorvem reflexos distantes.
  * *Geometria:* Malha quadriculada fortemente dividida (`DIVISOES = 30`).
  * *Categoria / Material Default:* PLANO / Cerâmica.

---

## 7. Sistema de materiais

O sistema provê física plausível para superfícies sem necessitar do cálculo massante manual. Cada primitivo reporta ao `GerenciadorMateriais` sob uma `CategoriaObjeto`. O Gerenciador mantém um vetor dinâmico local de predefinições (*presets*) de materiais com descrições ricas.

* **Armazenamento:** Dicionário dinâmico (`std::map`) vinculando categoria a `Material`. 
* **Presets existentes:**
  1. *Plástico Vermelho:* Reflexão difusa fortemente dominada pela cor, reflexão especular acromática (branca, vinda da luz) e *shininess* baixa.
  2. *Metal Prateado:* Oponente ao plástico — sua reflexão especular É tingida (metálica) e a luz difusa absorvida (baixa). Reflete um highlight altíssimo.
  3. *Borracha Azul:* Superfícies foscas. Zero especular, inteiramente regidas por Lambert (difuso).
  4. *Cerâmica Branca:* Intermediário, highlight esbranquiçado não focalizado e alta capacidade de receber luz de rebatimento (ambiente).
* **Parâmetros submetidos à GPU:** Ambient (4 floats), Diffuse (4 floats), Specular (4 floats), Shininess (1 float limite 128.0).
* **Interação:** O usuário pressiona `Tab` para ciclar qual categoria (ex: Cones) sofre as edições. Pressiona `4` para trocar de material na categoria, e usa `, / .` para expandir/concentrar o fator numérico de *shininess*.

### Tabela de Materiais

| Categoria | Geometria Original | Material Padrão Inicial | Materiais Disponíveis |
| :--- | :--- | :--- | :--- |
| ESFERA | 2 Esferas simétricas | Plástico Vermelho | Plástico, Metal, Borracha, Cerâmica |
| CONE | 2 Cones | Metal Prateado | Plástico, Metal, Borracha, Cerâmica |
| CUBO | 1 Cubo central | Borracha Azul | Plástico, Metal, Borracha, Cerâmica |
| PLANO | Chão massivo | Cerâmica (piso) | Plástico, Metal, Borracha, Cerâmica |

---

## 8. Sistema de iluminação

A iluminação neste programa utiliza a clássica pipeline fixa.

* **Luz Ambiente (`1`):** Retorna o "piso de luz", uma claridade onipresente que preenche toda a área impedindo escuridão absoluta. Neste modo, `luz.cpp` desliga os canais Difusos e Especulares de `GL_LIGHT0`, enviando zeros para o driver da GPU. O cálculo do driver se torna: Cor = k_a * L_a.
* **Modelo de Lambert / Difusa (`2`):** Adiciona-se a verificação cossenoide (produto escalar) entre o vetor Normal (N) e o vetor direcional à Fonte Luminosa (L). O código `luz.cpp` injeta o canal `GL_DIFFUSE` mas suprime o especular. O programa exibe a transição direcional suave de claridade baseada na proximidade/ângulo.
* **Modelo Blinn-Phong / Especular (`3`):** Todos os canais reativados (Ambiente + Difuso + Especular). A Fixed Function Pipeline do OpenGL costuma aproximar o vetor ideal de reflexão perfeita (R) utilizando um Vetor Bisectriz (Half-Vector, H), o que caracteriza o modelo **Blinn-Phong**. Ele capta a relação vetorial entre observador e luz, e acentua com potência shininess.

As trocas são feitas sob demanda dinamicamente enviando array de bytes com `0.0f` para os canais indesejados através da API `glLightfv`. A OpenGL avalia todos esses vetores **por vértice**, executando um *Gouraud Shading* onde as cores são interpoladas suavemente sobre a superfície dos polígonos adjacentes.

---

## 9. Câmera

A Câmera foi desenhada num modelo Orbital em coordenadas esféricas nativas `(azimute, elevação, distancia)`, traduzida ativamente com a biblioteca de trigonometria nativa para `x, y, z` usando senos e cossenos para o preenchimento na matriz LookAt do OpenGL (posição da câmera num hemisfério sempre fitando um `alvo` em x,y,z virtual e um up-vector `0,1,0`).

* **Controles:** Orbitagem contínua presa e zoom, garantindo que o usuário nunca se "perca" num mar 3D negro. 
* **Posicionamento Relativo (Pan):** Uma lógica matemática projeta vetores "frente" e "direita" puramente no plano XZ em relação ao ângulo esférico `azimute`, fazendo com que os inputs de tecla W, A, S e D sejam vetorialmente coerentes com o rumo que a visão da câmera aponta, deslizando a focalização para novas áreas.

---

## 10. Interação do usuário

| Controle | Categoria | Descrição da Ação |
| :--- | :--- | :--- |
| **Mouse Arrastar (Esq)** | Câmera | Move/Orbita a câmera em torno do centro do foco. |
| **Mouse Scroll** | Câmera | Aplica zoom, mudando o raio da distância entre a lente e o alvo. |
| **+/-** | Câmera | Idêntico ao mouse scroll. |
| **W, A, S, D** | Câmera | Mover-se lateralmente / frente-trás (Pan-Relativo). |
| **R** | Câmera | Reseta as posições e ângulos de visualização ao padrão de compilação. |
| **1** | Iluminação | Alterna apenas para o modelo Ambiente (base, s/ iluminação direcional). |
| **2** | Iluminação | Alterna para o modelo Lambertiano (Difuso). |
| **3** | Iluminação | Alterna para o modelo Blinn-Phong (Difuso + Especular). |
| **Setas Direcionais** | Iluminação | Navega fisicamente a coordenada da fonte de Luz (x e z). |
| **Page Up / Down** | Iluminação | Move fisicamente a coordenada da fonte de Luz (y/altura). |
| **[ / ]** | Iluminação | Regula a intensidade global fotométrica da fonte luminosa (escala rgb). |
| **Tab** | Materiais | Alterna de forma circular (ciclagem) as Categorias no terminal para ser a próxima passível de modificação. |
| **4** | Materiais | Aplica a substituição total do preset de material físico ao objeto selecionado. |
| **, / .** | Materiais | Decresce/aumenta a taxa de *shininess* (o brilho direcional da reflexão pontual do foco luminoso) da Categoria. |
| **ESC** | Janela | Encerra o projeto com código 0. |

---

## 11. Fluxo da renderização

Toda vez que a OpenGL avalia o frame via `glutPostRedisplay`:
1. `glClearColor` pinta a janela de cor base atmosférica (`0.1, 0.1, 0.12`).
2. Limpa o Framebuffer do Color (a tinta no frame antigo) e do Depth/Z-Buffer (a noção de profundidade pregressa) através de `glClear`.
3. Carrega uma matriz-identidade no ModelView.
4. Constrói o `LookAt` (Camera), reposicionando o referencial de origem global.
5. Invoca a classe `Luz`, definindo fisicamente com `glLightfv` onde a coordenada matemática local se fixará no espaço e quais valores luminosos (amb, difuso, spec) operarão neste frame.
6. A `Cena` interage no iterador com seus `Objetos`.
7. O loop do Objeto preserva a matriz (`glPushMatrix()`), move fisicamente sua coordenada local (`glTranslate`), consulta o Material via singleton injetando seus metadados `glMaterial` à GPU, executa os *batches* geométricos puros, e por fim reseta o estado da matriz para não vazar a próxima instrução geométrica (`glPopMatrix`).
8. O `MarcadorLuz` é desenhado (uma esfera estática visualizando a lâmpada).
9. O contexto duplo exibe o render para a janela com `glutSwapBuffers()`.

---

## 12. Organização do código

O código é altamente modularizado para um projeto OpenGL estrito, focando no desacoplamento. 

* `main.cpp`: Possui imensa responsabilidade de *glue-code* (código cola) da API freeGLUT mas quase nenhuma responsabilidade física e matemática de estado; é o reator passivo.
* `gerenciador_materiais.cpp / .h`: Tem grande responsabilidade semântica, contendo a inteligência e abstração de um motor moderno (gerir classes isoladas independentes da classe `Objeto`).
* `geometria/*.cpp`: Classes puramente coesas e auxiliares, responsáveis pela trigonometria formadora das malhas.
* As dependências são hierárquicas invertidas: o `Objeto` (Geometria) não é dono do seu `Material`, ele pesquisa ao singleton no momento da injeção, reduzindo o acoplamento global, evitando ponteiros repetitivos ou referências soltas.

---

## 13. Decisões de projeto

* **Materiais em Singleton/Categoria:** Decidido para prevenir que uma tela complexa perdesse coerência lumínica. Ao invés do ponteiro pertencer ao objeto local, o singleton garante que todos os "Cones" renderizem obrigatoriamente perfeitamente o "Metal" no mesmo referencial.
* **Geometria Subdividida de Primitivas Planas (Cubo e Plano):** Essencial e intencional. Ao invés de um plano de chão renderizado por apenas quatro vértices longínquos (`glVertex3f` único gigante), a decisão projetual usa loops de subdivisão (e.g. `DIVISOES=30` no piso e 8x8 nas faces do cubo). Isso ocorre pois o Gouraud Shading calcula a matemática de cor no Vértice. Sem malha densa, os polígonos imensos perderiam destaques do termo Especular, uma vez que a luz concentrada atingiria o meio do polígono e os quatro vértices externos nunca estariam num ângulo de colisão preciso com a luz, apagando o realismo.
* **Câmera Orbital vs. First-Person:** Euler angles para first person acarretam instabilidade em interações simples por arrasto. A escolha pelas coordenadas radiais restritas esféricas previne comportamentos patológicos da matriz modelview.

---

## 14. Dificuldades técnicas (inferidas pelo código)

* **Contenção do Gimbal Lock e Cálculos Tridimensionais:** O código denota cálculos vetoriais densos no arquivo `camera.cpp` para evitar translações equivocadas (Pan) num sistema esférico e limitação em +-89º de *Pitch*.
* **Acurácia Fotométrica via Fixed Pipeline:** A OpenGL tradicional simplifica muito e omite perfeitamente comportamentos físicos; a precisão de um brilho de superfície é prejudicada quando malhas têm polígonos enormes. O desafio notado e resolvido foi adaptar manualmente `Cubo` e `Plano` para subdivisões densas forçadas (`GL_QUADS` interativos via grids menores).
* **Geração Analítica da Geometria:** Observa-se matemática vetorial avançada gerando normais não automáticas no arquivo do cone (`N(theta) = normalize(altura*cos(theta), raio, altura*sin(theta))`) onde o programa manualmente cria tiras triângulos `GL_TRIANGLE_STRIP` precisas por trigonometria, necessitando cuidado redobrado sobre as orientações anti-horárias da Fixed Pipeline.

---

## 15. Limitações atuais

* **Vertex-Based Shading (Gouraud Shading):** A dependência global ao sombreamento Gouraud gera artefatos poligonais leves e uma especularidade que jamais ficará perfeitamente circular ou lisa como um sombreamento avançado baseado em fragmentos (Per-pixel lighting, Phong Shading original). 
* **Tesselation Excessiva:** Para resolver o *Vertex-Based*, os polígonos necessitam ser altamente subdivididos nas classes C++, inflando artificialmente os drawcalls da placa de vídeo.
* **Manejo Estrito de Estado:** Depender das instâncias puras de OpenGL state machines (máquinas de estado).
* **Luz Pontual Única:** Atualmente codificado hardcoded apenas a abstração total da `GL_LIGHT0`. O suporte simultâneo a múltiplas lâmpadas espalhadas exigiria uma recodificação drástica da classe `Luz`.

---

## 16. Possíveis melhorias

* **Melhorias simples:** 
  * Adicionar UI visual nativa (Overlay OSD usando texto glutBitmapString sobre render) no lugar dos console prints.
  * Inclusão de um novo modo de renderização *wireframe* no `main.cpp` via `glPolygonMode` para avaliação técnica das subdivisões poligonais (já que foi um problema notado na concepção).
* **Melhorias moderadas:** 
  * Adição de texturização plana sobre a base do chão ou esferas, usando UV Mapping calculado perfeitamente nas malhas já parametrizadas.
  * Refatoração arquitetônica para suporte de arrays de Luz (habilitando `GL_LIGHT1`, etc).
* **Melhorias avançadas:**
  * Remoção do OpenGL Legacy (FFP) base e transição integral para Modern OpenGL (Core Profile) instanciando Shaders (GLSL) autônomos no processo das matrizes (Phong Shading via per-pixel computation) removendo a dor de cabeça com subdivisão de geometria bruta.
  * *Shadow mapping* em tempo real.

---

## 17. Experimentos recomendados

* **Experimento 1: A Importância do Reflexo Direcional**
  * *Objetivo:* Provar a diferença base fotométrica do material condutor vs dielétrico isolante.
  * *Parâmetros:* Definir Modo = 3 (Blinn-Phong). Aplicar material Plástico vs. Metal num mesmo objeto (Categoria: Cones).
  * *Imagens:* Uma imagem mostrando Plástico; Uma Imagem mostrando Metal na mesma pose e iluminação.
  * *Análise esperada:* Avaliar que o Plástico desvia do pigmento colorido na especularidade (reflete branco/luz da sala crua) e mantém alto espalhamento difuso, enquanto o Metal exibe especularidade amarela forte colorida na absorção, provando o uso funcional dos parâmetros ambient/diffuse/specular de forma não unificada.
* **Experimento 2: Avaliação progressiva da luz de Lambert e Phong (Foco no Brilho Especular)**
  * *Objetivo:* Relacionar as parcelas da equação de iluminação e o visual renderizado, destacando explicitamente a injeção do pico especular.
  * *Parâmetros:* Categoria: Esferas. Com Material de **Plástico Vermelho** ou **Metal Prateado**, transitar progressivamente usando 1 (Ambiente) e capturar imagem, depois 2 (Lambert/Difuso) e capturar, e enfim 3 (Blinn-Phong/Completo) e capturar.
  * *Imagens:* Três imagens estáticas de progresso incremental das esferas.
  * *Análise esperada:* Mostrar visualmente o acúmulo fotométrico: A luz ambiente (1) gera um disco opaco bidimensional; a luz difusa (2) revela o volume 3D da esfera através do sombreamento suave (Lambert); e por fim, o modo especular (3) (Blinn-Phong) adiciona o *highlight* (reflexo do ponto de luz) concentrado sobre a superfície curvada, provando o funcionamento da componente que depende da posição da câmera.

---

## 18. Capturas de tela recomendadas

Para suportar teoricamente a escrita do relatório:

* **Visão Panorâmica (Blinn-Phong Geral):** Câmera recuada com todos os objetos aparentes no ambiente. *(Sessão: Visão Geral / Final results).*
* **Isolamento de Modos da Pipeline Fixa:** Capturas do plano focando num único objeto sob modos 1, 2 e 3 consecutivamente. *(Sessão: Sistema de Iluminação).*
* **Highlights Especulares:** Captura focando detalhadamente sobre o topo do Cone Metálico para debater sobre *shininess* (coeficiente de rugosidade / reflexão especular acentuada) e absorção metálica. *(Sessão: Sistema de Materiais).*
* **Plano vs Esfera de Gouraud:** Aproximação entre a divisão natural entre vértices num cubo (faces achatadas e planas contrastantes da especularidade) vs malhas contínuas (a esfera que interpola cores sobre a face). Discute-se a subdivisão artificial criada no código para corrigir limitação visual em faces largas do chão. *(Sessão: Decisões e Dificuldades de Arquitetura / Arquitetura Geometria).*

---

## 19. Glossário

* **Rendering:** O processo sistemático efetuado por um hardware ou software visando gerar uma representação de imagem (raster) baseada unicamente num conjunto numérico de matrizes paramétricas 3D.
* **Modelo de iluminação local:** Modelo prático clássico, assumindo simplificações (as sombras auto-projetadas ou rebatimento dinâmico não atuam no equacionamento base); simula luz batendo e voltando direto ao observador.
* **Normal (N):** Vetor estritamente perpendicular a um referencial na face de um polígono ou vértice interpolado. O principal parâmetro basilar do cálculo fotométrico simulado computacional (define se o polígono recebe luz direta de uma fonte).
* **Material (Propriedade Física na GPU):** Representação matemática que codifica quão sensível um vértice/polígono é contra feixes de luz, distribuída entre coeficientes refletivos de difusão ou espelhamento.
* **Fixed Function Pipeline (FFP):** Etapas clássicas estáticas imbutidas nativamente do controlador gráfico da GPU num software (OpenGL legado e antigos hardwares não-programáveis) baseadas no preenchimento de estados fixos de processamento (`glLight`, `glMaterial`). Não-programável (ou seja, você não escrevia explicitamente o código das cores a serem processadas no final, você cedia os números de base e a GPU fazia).
* **Gouraud Shading:** Técnica de sombreamento suave implementada calculando as cores por vértices poligonais na extremidade e misturando linearmente os dados na travessia interna. Possui falhas para especularidades severas finas.
* **Lambert:** Comportamento matemático direcional descrevendo luz difusa; postula que a luz se reflete em múltiplas direções independente do vetor exato que a câmera aponta; puramente ditado pelo produto vetorial cosseno entre direção da emissão luminosa em relação a orientação espacial plana do referencial normativo da face.
* **Phong (Reflexão):** Modelo clássico especular (o clássico "brilho"). Adiciona um fator direcional puro para criar "pontos agudos de brilho de luz" (espalhamento microscópico de superfície condutora/polida). Põe o cálculo do reflexo verdadeiro da luz e comparação do cosseno entre o vetor da visão do espectador contra a angulação.
* **Blinn-Phong:** Simplificação analítica da técnica autoral de Bui Tuong Phong visando diminuir peso computacional ao substituir cálculo contínuo vetorial reflexivo perfeito por cálculos utilizando um simples vetor intermediário estático central (Half-Vector), permitindo precisão aceitável na *Fixed Function Pipeline*.

---

## 20. Informações para quem irá escrever o relatório

Caro parceiro encarregado da redação:

* **Arquitetura Base:** Enfatize brutalmente a refatoração. Todo sistema de materiais neste projeto se baseia numa inversão de controle inteligente e *Singleton*. Em um projeto clássico escolar OpenGL, a cor pertence ao cubo. Aqui, a cor pertence à **categoria** `CUBO` sob domínio do orquestrador global (`GerenciadorMateriais`), isso demonstra sofisticação em *Design Patterns* no ecossistema C++ voltado a Gráficos 3D interativos e escalabilidade de manutenção.
* **Câmera:** Sublinhe que o uso de coordenadas esféricas polares (Orbital Camera) com controle *Pan* baseado em vetorização relativa (frente projetada sobre plano XZ) evita a famosa restrição degenerativa de *Gimbal Lock* que frustra projetos baseados puramente em Matriz de Euler (eixos x,y,z hardcoded) na exploração interativa num cenário de OpenGL Clássico.
* **Desafio Gouraud Shading:** **EXTREMO CUIDADO NESTE PONTO**. O projeto utiliza a arquitetura estática da OpenGL clássica. Portanto, ele se afasta do Modern Shader per-pixel calculation, ficando refém da interpolação de iluminação por Vértice. Saliente na escrita (pois foi um esforço técnico visível e contornado no código via classes Plane e Cube) que faces chapadas gigantes impossibilitam a absorção precisa de um brilho fino especular. Escreva extensamente que a solução pragmática do código base (criada de forma analítica nas classes `cubo.cpp` e `plano.cpp` através do agrupamento maciço fracionário de `GL_QUADS`) serviu unicamente para subdividir, e forçar, que as luzes recaíssem sobre dezenas de novos vértices interpoladores, gerando uma reflexão bonita e simulada mesmo rodando nas entranhas da máquina Fixed-Function do OpenGL legado. Use a explicação técnica do Glossário para inflar positivamente a percepção de proeficiência gráfica!
