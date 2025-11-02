### Trabalho Prático I — Pista 2D

Tema  
Simulação de uma pista top‑down 2D

Descrição da Proposta  
O presente projeto consiste numa simulação interativa de um ambiente Top-Down 2D, onde um carro (representado por um triângulo) tem que desviar de círculos que aparecem de forma aleatoria no cenário.
- Cenário
    - A pista possui traçado fixo, delimitada por um retângulo de jogo, contendo bordas.
    - O carro do jogador é representado em 2D e se movimenta com base em transformações geométricas (translação, rotação) ao longo do traçado.
    - Objetos são criados aleatoriamente na pista para que colidam com o veículo.
    - Textos no HUD exibem informações dinâmicas:
        - Velocidade atual do carro.
        - Contador de colisões com buracos.
    - Existe uma linha amarela centralizada que serve para simular movimento do carro na pista.
    - O fundo do ambiente de movimento do carro tem a textura de asfalto.

- Simulação
    - A simulação é contínua, com atualização de estado a cada quadro: física básica do carro (aceleração, frenagem (basta parar de usar a seta), limitação de velocidade) e detecção de eventos.
    - Colisões:
        - Com círculos: incrementam o contador de falhas; ao atingir 3 colisões, o jogador perde a corrida.
        - Com limites: ao chegar em algum limite o carro não pode ultrapassar.
        - Colisões com círculos fazem com que o carro fique vermelho, ao bater 5 vezes, encerra. 

- Interação
    - O usuário pode interagir com a simulação por meio de comandos de teclado:
        - Acelerar: W ou seta para cima
        - Frear: S ou seta para baixo
        - Virar à esquerda: A ou seta para esquerda
        - Virar à direita: D ou seta para direita
        - Pausar/Retomar: P
        - Resetar posição/volta corrente: R
        - Sair: Esc
