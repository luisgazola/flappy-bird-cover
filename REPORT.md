# Relatório de estado — Flappy Bird Cover

**Data:** 27/11/2026 · **Referência:** `main` em `22e34cd` (merge do PR #1, `refactor/nucleo-testavel-sfml3`)

Relatório da revisão de documentação. Os resultados abaixo foram **copiados do README**; esta revisão não compilou nem executou o jogo.

## Resumo

Jogo completo de desviar de canos, com estados, física em passo fixo, dificuldade progressiva, recorde persistente, pausa e contador de FPS. Núcleo sem SFML coberto por testes; CI no Ubuntu 24.04.

## Testes registrados

| Validação | Resultado | Tipo |
| :--- | :--- | :--- |
| `tests/core_tests.cpp` | 15/15 (Gentoo, GCC 16.2.1) | Automatizado |
| Build com avisos estritos | Sem avisos | Automatizado |
| CI no Ubuntu 24.04 | `make test` e build aprovados | Automatizado |
| Partidas reais | Jogadas pelo autor | Validação manual |
| Camada SFML (`src/app/`) | Sem testes | — |

## Riscos e inconsistências

| # | Onde | Descrição |
| :--- | :--- | :--- |
| 1 | **`resources/`** | Sprites e fonte de origem e licença desconhecidas, provavelmente do jogo original, num repositório **público**; não cobertos pela licença MIT |
| 2 | **Marca** | O nome do projeto usa "Flappy Bird", marca de terceiros |
| 3 | `~/Projetos/flappy-bird-cover/flappy-bird-cover-main/` | Cópia antiga do projeto, sem Git, fora deste repositório; não recebeu a nova documentação |
| 4 | Branch | `refactor/nucleo-testavel-sfml3` (local e remota) já foi mergeada e pode ser removida |
| 5 | Licença | Não havia licença; o `LICENSE` MIT só para o código foi criado por decisão do autor em 27/11/2026 |

## Recomendações

1. Substituir os recursos de `resources/` por arte e fonte próprias (uma fonte com acentos permitiria acentuar a interface).
2. Avaliar renomear o projeto para evitar o uso da marca.
3. Arquivar ou remover a cópia `flappy-bird-cover-main/` depois de conferir que nada nela falta no repositório.
