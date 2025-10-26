### Corrida 2D — Computação Gráfica Cefet-MG

Simulação interativa de uma corrida top‑down 2D com foco em transformações geométricas, atualizações contínuas de estado e HUD informativo. O projeto foi concebido para cumprir os requisitos do Trabalho Prático I (Computação Gráfica).

#### Sumário
- Contexto e Objetivo
- Interações do Usuário (Controles)
- Regras de Negócio
- Requisitos Funcionais
- Organização Arquitetural do Projeto
- Estrutura de Diretórios
- Fluxo de Execução e Componentes

---

### Contexto e Objetivo
- O projeto implementa um cenário 2D de corrida top‑down usando OpenGL e FreeGLUT em C++, com:
    - Desenhos 2D e transformações geométricas (translação/rotação/escala).
    - Atualização contínua da simulação (movimento, detecção, contagem de voltas).
    - HUD com informações dinâmicas (voltas, tempo, velocidade).
    - Interações via teclado.
- O design separa regras de domínio da infraestrutura gráfica, favorecendo manutenção e migração fácil para 3D.

---

### Interações do Usuário (Controles)
- Acelerar: W ou Setinha para cima
- Frear: S ou Setinha para baixo
- Virar à esquerda: A ou Setinha para esquerda
- Virar à direita: D ou Setinha para direita
- Aceleração especial (+25% por 3s, até 3 usos por corrida): Barra de Espaço
- Pausar/Retomar: P
- Reset: R
- Sair: Esc

---

### Regras de Negócio
As regras de negócio definem o comportamento do sistema de corrida:

1) Pista e Limites
- A pista possui formato fixo e é delimitada por um retângulo de jogo; sair dos limites é tratado como colisão.
- A pista não utiliza checkpoints; o progresso é medido exclusivamente por voltas completas no traçado.

2) Progresso e Voltas
- A volta é considerada completa quando o carro cruza a linha de largada/chegada no sentido correto.
- Ao completar uma volta, o cronômetro da volta atual é finalizado e comparado ao melhor tempo.
- O contador de voltas é incrementado a cada volta válida concluída.

3) Colisões e Falhas
- Não há outros carros. “Obstáculos” são buracos gerados aleatoriamente sobre a pista.
- Colidir com um buraco incrementa um contador de falhas; ao atingir 3 colisões, o jogador perde a corrida.
- Colisão com limites da pista também pode reduzir velocidade e/ou reposicionar o carro de forma conservadora (sem invalidar voltas já concluídas).

4) Física e Movimento
- O carro possui aceleração progressiva no eixo frontal, frenagem que reduz a velocidade na direção atual, arrasto e limitação de velocidade máxima.
- A taxa de rotação do carro depende da velocidade (virar parado é menos responsivo).
- Aceleração especial: até 3 ativações por corrida, cada uma aplicando +25% na velocidade (ou velocidade máxima efetiva) por 3 segundos; ativações não se acumulam além do efeito vigente (renova/estende duração se ativado durante efeito ativo, consumindo uma carga).

5) HUD e Métricas
- O HUD exibe: voltas atuais/target, tempo da volta atual, melhor volta, velocidade atual, cargas restantes de aceleração especial e contador de colisões com buracos.
- Os tempos são reiniciados ao completar uma volta válida.
- As informações do HUD devem ser legíveis em tempo de execução, sempre atualizadas a cada quadro.

6) Estados e Reset
- O jogo possui estado de pausa (toggle).
- Reset recoloca o carro na linha de largada/chegada, zera a velocidade, reinicia o tempo de volta corrente e mantém coerência de estatísticas já consolidadas (melhor volta permanece; voltas concluídas podem ser preservadas conforme política da corrida, típica: reset parcial sem reiniciar a corrida).

7) Entrada e Mapeamento
- A aceleração, frenagem e esterço (esquerda/direita) podem ser dirigidos por teclas WASD e/ou setas direcionais.
- Aceleração especial é acionada pela Barra de Espaço, consumindo uma carga disponível.
- Teclas de sistema: Pausa (P), Reset (R), Sair (Esc).

8) Robustez e Consistência
- A detecção de volta utiliza linha de largada/chegada e direção de cruzamento para evitar contagens indevidas.
- A geração de buracos respeita a área da pista e evita sobreposição excessiva e posições impossíveis.
- Colisões e resets nunca devem causar estados inválidos (posições fora dos limites sem correção, contadores negativos, etc.).

---

### Requisitos Funcionais
- RF01: Renderizar a pista 2D de formato fixo, com limites visuais e linha de largada/chegada.
- RF02: Controlar o carro com aceleração, frenagem e esterço, atualizando posição e orientação continuamente.
- RF03: Detectar colisões com limites da pista e com buracos, aplicando penalidades e contabilizando falhas.
- RF04: Medir progresso de corrida por número de voltas e detectar cruzamento válido da linha de largada/chegada.
- RF05: Temporizar a volta atual e registrar o melhor tempo.
- RF06: Exibir HUD com voltas, tempo da volta, melhor volta, velocidade, cargas de aceleração especial e contador de colisões.
- RF07: Permitir pausar e retomar a simulação.
- RF08: Permitir reset do carro para a linha de largada/chegada.
- RF09: Implementar aceleração especial de +25% por 3 segundos com até 3 usos por corrida (comportamento de renovação/ extensão quando acionado durante efeito ativo).
- RF10: Suportar entrada de teclado (WASD/Setas, Espaço, P, R, Esc).

---

### Organização Arquitetural do Projeto
A aplicação é estruturada em camadas bem definidas:

- Domain (Regras de Domínio):
    - Entidades, estados e regras: carro, física, pista fixa, linha de largada/chegada, buracos (obstáculos), cronômetro de voltas, aceleração especial.
    - Independente de OpenGL/GLUT.

- Application (Orquestração):
    - Loop do jogo, tempo (delta), mapeamento de entradas para comandos, ciclo update -> render.
    - Coordena a UI e o domínio.

- Infrastructure (Infraestrutura Gráfica):
    - Janela OpenGL/FreeGLUT, callbacks, setup de projeção ortográfica, primitivas de desenho 2D.
    - Fornece utilitários de renderização e integração com eventos.

- UI (Apresentação):
    - Composição de cena e HUD.
    - Invoca o renderer para desenhar elementos de domínio.

Essa separação permite que a lógica de simulação seja mantida ao trocar a camada de visualização para 3D.

---

### Estrutura de Diretórios
- CMakeLists.txt
- src/
    - main.cpp
    - app/
        - GameApp.h / GameApp.cpp — orquestra aplicação, loop, callbacks GLUT, integração input->domínio.
        - InputMapper.h / InputMapper.cpp — mapeia teclas para controles do carro (inclui Espaço para aceleração especial).
        - Time.h / Time.cpp — controle de delta time.
    - domain/
        - types/MathTypes.h — tipos matemáticos básicos (vetor, utilidades).
        - car/
            - Physics.h — integração física do carro (aceleração, freio, arrasto, esterço).
            - Car.h — entidade Car, estado, aceleração especial (cargas/duração) e atualização.
            - LapTimer.h — cronometragem de voltas (atual, melhor).
        - track/
            - Track.h / Track.cpp — limites, linha de largada/chegada, geração de buracos aleatórios, contagem de voltas.
    - infrastructure/
        - GlWindow.h / GlWindow.cpp — inicialização GLUT, callbacks e janela.
        - GlRenderer2D.h / GlRenderer2D.cpp — projeção ortográfica e primitivas 2D (retângulos, círculos, triângulos, texto).
        - Texture.h / Texture.cpp — reservado para futura textura (opcional).
    - ui/
        - Scene2D.h / Scene2D.cpp — composição da cena (pista fixa, buracos, carro).
        - Hud.h / Hud.cpp — renderização das informações de status (voltas, tempos, velocidade, cargas, falhas).
- assets/
    - textures/ — imagens para pista, bordas, carro (opcional nesta fase).

---

### Fluxo de Execução e Componentes
1) Inicialização
- `GlWindow::init` cria janela e contexto OpenGL.
- `GameApp` registra callbacks (display, idle, reshape, teclado).
- Setup de projeção ortográfica para coordenadas de mundo 2D.
- Pista fixa é construída; buracos são gerados aleatoriamente sobre a área válida da pista.

2) Loop (cada quadro)
- Idle:
    - Calcula `dt` (Time).
    - Lê estado de teclas e preenche `CarInputState` (InputMapper), incluindo ativação da aceleração especial.
    - Atualiza `Scene2D`, que:
        - Atualiza física do carro (Domain/Physics).
        - Aplica efeito da aceleração especial quando ativo (modificando velocidade/limite por 3s).
        - Detecta cruzamento válido da linha de largada/chegada e contabiliza voltas (LapTimer).
        - Detecta colisões com limites e com buracos; incrementa contador de falhas e finaliza corrida ao atingir 3 colisões.
- Display:
    - Limpa tela e configura ortho.
    - Desenha pista fixa, bordas e linha de largada/chegada.
    - Desenha buracos aleatórios.
    - Desenha o carro.
    - Desenha HUD (voltas, tempos, velocidade, cargas restantes, falhas).

3) Interação
- Teclas ajustam `CarInputState` e comandos (aceleração especial, pausa, reset, sair).
- Reset posiciona o carro na linha de largada/chegada, zera a velocidade e reinicia tempo de volta corrente.