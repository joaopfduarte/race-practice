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

### Regras de Negócio
As regras de negócio definem o comportamento do sistema de corrida:

1) Pista e Limites
- A pista é delimitada por um retângulo de jogo; sair dos limites é tratado como colisão.
- Checkpoints são regiões circulares em sequência e determinam o progresso de volta.
- Boost Zones (zonas de impulso) são regiões circulares que aumentam temporariamente a velocidade máxima do carro.

2) Progresso e Voltas
- A volta é considerada completa quando o jogador atravessa todos os checkpoints na ordem definida e retorna ao primeiro.
- Ao completar uma volta, o cronômetro da volta atual é finalizado e comparado ao melhor tempo.
- O contador de voltas é incrementado a cada volta válida concluída.

3) Colisões
- Colisão com obstáculos (círculos) ou com os limites da pista provoca reposicionamento do carro no último checkpoint validado.
- A colisão não incrementa contadores, não altera diretamente a volta a menos que afete a ordem de checkpoints; a penalização é o reposicionamento + perda de velocidade.

4) Física e Movimento
- O carro possui aceleração progressiva no eixo frontal, frenagem que reduz a velocidade na direção atual, arrasto e limitação de velocidade máxima.
- A taxa de rotação do carro depende da velocidade (virar parado é menos responsivo).
- O boost aumenta a velocidade máxima enquanto o carro estiver dentro da zona de impulso; efeitos não são cumulativos além da soma das zonas simultâneas.

5) HUD e Métricas
- O HUD exibe: voltas atuais/target, tempo da volta atual, melhor volta e velocidade atual.
- Os tempos são reiniciados ao completar uma volta válida.
- As informações do HUD devem ser legíveis em tempo de execução, sempre atualizadas a cada quadro.

6) Estados e Reset
- O jogo possui estado de pausa (toggle).
- Reset recoloca o carro no último checkpoint validado (ou no primeiro, caso nenhum tenha sido validado), zera a velocidade e mantém estatísticas coerentes (voltas e melhor tempo preservados; tempo da volta reinicia).

7) Entrada e Mapeamento
- A aceleração, frenagem e esterço (esquerda/direita) podem ser dirigidos por teclas WASD e/ou setas direcionais.
- Teclas de sistema: Pausa (P), Reset (R), Sair (Esc).

8) Robustez e Consistência
- O sistema deve manter a ordem de checkpoints; pular um checkpoint invalida o progresso até o jogador retornar à sequência correta.
- Colisões e resets nunca devem causar estados impossíveis (posições fora da pista, índices inválidos, etc.).

---

### Requisitos Funcionais
- RF01: Renderizar a pista 2D com limites visuais, checkpoints, obstáculos e zonas de boost.
- RF02: Controlar o carro com aceleração, frenagem e esterço, atualizando posição e orientação continuamente.
- RF03: Detectar colisões com obstáculos e com limites da pista, reposicionando o carro conforme regra.
- RF04: Gerenciar checkpoints em ordem, contabilizando voltas concluídas corretamente.
- RF05: Temporizar a volta atual e registrar o melhor tempo.
- RF06: Exibir HUD com voltas, tempo da volta, melhor volta e velocidade.
- RF07: Permitir pausar e retomar a simulação.
- RF08: Permitir reset do carro para o último checkpoint válido.
- RF09: Aplicar efeitos de boost nas áreas definidas.
- RF10: Suportar entrada de teclado (WASD/Setas, P, R, Esc).

---

### Requisitos Não Funcionais
- RNF01: Código em C++20, organizado por camadas (Domínio, Aplicação, Infraestrutura e UI).
- RNF02: Usar OpenGL + FreeGLUT para janela, contexto e entrada.
- RNF03: Taxa de atualização contínua; dt limitado para estabilidade numérica.
- RNF04: Portabilidade conceitual para 3D (lógica de domínio desacoplada da renderização).
- RNF05: Compilação via CMake; execução no CLion (JetBrains) em Windows (paths ajustáveis).

---

### Organização Arquitetural do Projeto
A aplicação é estruturada em camadas bem definidas:

- Domain (Regras de Domínio):
    - Entidades, estados e regras: carro, física, pista, checkpoints, obstáculos, boost zones, cronômetro de voltas.
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
        - InputMapper.h / InputMapper.cpp — mapeia teclas para controles do carro.
        - Time.h / Time.cpp — controle de delta time.
    - domain/
        - types/MathTypes.h — tipos matemáticos básicos (vetor, utilidades).
        - car/
            - Physics.h — integração física do carro (aceleração, freio, arrasto, esterço).
            - Car.h — entidade Car, estado e atualização.
            - LapTimer.h — cronometragem de voltas (atual, melhor).
        - track/
            - Track.h / Track.cpp — limites, checkpoints, obstáculos, boosts e progresso.
    - infrastructure/
        - GlWindow.h / GlWindow.cpp — inicialização GLUT, callbacks e janela.
        - GlRenderer2D.h / GlRenderer2D.cpp — projeção ortográfica e primitivas 2D (retângulos, círculos, triângulos, texto).
        - Texture.h / Texture.cpp — reservado para futura textura (opcional).
    - ui/
        - Scene2D.h / Scene2D.cpp — composição dos elementos da cena (pista, carro, objetos).
        - Hud.h / Hud.cpp — renderização das informações de status.
- assets/
    - textures/ — imagens para asfalto, grama, zebra, carro, cones (opcional nesta fase).

---

### Fluxo de Execução e Componentes
1) Inicialização
- `GlWindow::init` cria janela e contexto OpenGL.
- `GameApp` registra callbacks (display, idle, reshape, teclado).
- Setup de projeção ortográfica para coordenadas de mundo 2D.

2) Loop (cada quadro)
- Idle:
    - Calcula `dt` (Time).
    - Lê estado de teclas e preenche `CarInputState` (InputMapper).
    - Atualiza `Scene2D`, que:
        - Atualiza física do carro (Domain/Physics).
        - Consulta boosts e aplica `maxSpeed` temporário.
        - Atualiza progresso em checkpoints e volta (LapTimer).
        - Verifica colisões e reposiciona quando necessário.
- Display:
    - Limpa tela e configura ortho.
    - Desenha limites de pista, checkpoints, obstáculos, boosts.
    - Desenha o carro.
    - Desenha HUD (voltas, tempos, velocidade).

3) Interação
- Teclas ajustam `CarInputState` e comandos (pausa, reset, sair).
- Resets preservam consistência de estado.

---

### Interações do Usuário (Controles)
- Acelerar: W ou Setinha para cima
- Frear: S ou Setinha para baixo
- Virar à esquerda: A ou Setinha para esquerda
- Virar à direita: D ou Setinha para direita
- Pausar/Retomar: P
- Reset: R
- Sair: Esc

---

### Compilação e Execução (CMake/CLion)
1) Dependências
- CMake >= 3.16
- OpenGL (opengl32, glu32 no Windows)
- FreeGLUT
- Compilador com C++20

2) CMakeLists.txt (exemplo)
- Ajuste `include_directories` e `link_directories` conforme seu ambiente MSYS/MinGW/FreeGLUT.
- Exemplo base:
```
cmake_minimum_required(VERSION 3.16)
project(graphical_practice)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

include_directories("C:/msys64/mingw64/include")
link_directories("C:/msys64/mingw64/lib")

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(graphical_practice ${SRC_FILES})
target_include_directories(graphical_practice PRIVATE src)

target_link_libraries(graphical_practice PRIVATE
    OpenGL::GL
    freeglut
    opengl32
    glu32
    winmm
    gdi32
    user32
)
```

3) CLion
- Abra o diretório do projeto no CLion.
- Configure o Toolchain (MinGW/MSYS2 conforme seu setup).
- Build e Run o target `graphical_practice`.

---

### Próximas Extensões (Sugeridas)
- Texturas e Materiais:
    - Carregar texturas (stb_image) e desenhar quads texturizados para pista/grama/decoração.
- Geometria de Pista:
    - Curvas com polilinhas e máscaras de colisão por segmentos (em vez de retângulo global).
- Colisão e Física:
    - Melhorar modelo de atrito lateral/derrapagem, limites com paredes, cones como círculos sólidos/fill.
- UI/HUD:
    - Refinar tipografia, adicionar contador de checkpoints restantes, mini‑map.
- Portabilidade para 3D:
    - Substituir `GlRenderer2D` por renderer 3D, extrudar formas e usar câmera isométrica/perspectiva mantendo o domínio intacto.

---

### Créditos e Licença
- Uso acadêmico para a disciplina de Computação Gráfica.
- Bibliotecas e assets visuais devem respeitar suas licenças específicas (ex.: FreeGLUT, stb_image, texturas).