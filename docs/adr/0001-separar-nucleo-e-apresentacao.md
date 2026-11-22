# ADR 0001 — Separar núcleo do jogo e apresentação

- Status: aceito
- Data: 2026-11-21

## Contexto

A versão inicial concentrava tudo na classe `FlappyBird`: janela, leitura de teclado, física, canos, colisão, pontuação e desenho. Isso causava:

- física dependente da taxa de quadros (valores em pixels por quadro);
- regras impossíveis de testar sem abrir uma janela SFML;
- bugs difíceis de isolar (pontuação baseada em `x == 448`, canos inferiores gerados fora da tela, pássaro sem limite de chão, quadro de animação fora da textura).

## Decisão

1. `src/core/` contém as regras do jogo em C++17 puro, sem incluir SFML: `World` (máquina de estados `Ready`/`Playing`/`Paused`/`GameOver`), `Bird`, `PipePair`, `Rect`, `HighScoreStore` e `GameConfig.hpp`.
2. `src/app/Game` apenas traduz eventos SFML em chamadas a `World` e desenha o estado atual.
3. A simulação usa unidades por segundo e avança em passo fixo (1/120 s) com acumulador; o tempo de quadro é limitado a 0,25 s.
4. A aleatoriedade é injetada por semente (`World(seed)`), permitindo testes reprodutíveis.

## Consequências

- `make test` valida a lógica sem SFML nem servidor gráfico.
- Ajustes de jogabilidade ficam em `GameConfig.hpp`.
- A camada SFML continua sem testes automatizados; sua validação é manual.
- A migração do SFML 2 para o SFML 3 alterou apenas `src/app/`, sem tocar no núcleo.
