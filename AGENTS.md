# AGENTS.md — Flappy Bird Cover

Jogo 2D em C++17 com SFML 3, projeto de estudo. Repositório: github.com/luisgazola/flappy-bird-cover.

## Stack

- C++17, GNU Make, `pkg-config`
- SFML 3 (`sfml-graphics`); SFML 2 é incompatível e o Makefile recusa
- Ambiente validado: Gentoo Linux, GCC 16.2.1, SFML 3.0.1 (`media-libs/libsfml`)

## Arquitetura

- `src/core/`: regras do jogo em C++ puro. **Não incluir SFML aqui.**
  - `World`: máquina de estados (`Ready`, `Playing`, `Paused`, `GameOver`), canos, colisão, pontuação, dificuldade
  - `Bird`, `PipePair`, `Rect`, `HighScoreStore`
  - `GameConfig.hpp`: todas as constantes de jogabilidade (px, s, px/s)
- `src/app/Game`: janela, eventos, game loop de passo fixo (1/120 s) e desenho
- `tests/core_tests.cpp`: testes do núcleo, sem framework externo (macro `CHECK`)
- Decisões em `docs/adr/`

## Comandos

```bash
make            # compila ./flappy
make run        # compila e executa (rodar da raiz: recursos em resources/)
make test       # testes do núcleo; não precisa de SFML
make clean
make SFML_PREFIX=<dir>          # SFML fora do sistema
scripts/build-sfml.sh <dir>     # compila SFML 3.0.1 em <dir>
```

## Regras

- Mudança de comportamento em `src/core` exige teste em `tests/core_tests.cpp`.
- Compilar sem avisos (`-Wall -Wextra -Wpedantic -Wshadow -Wconversion`).
- A fonte `flappybird.ttf` não tem acentos: textos da interface sem acentuação.
- Recorde fica em `$XDG_DATA_HOME/flappy-bird-cover/highscore.txt`; não versionar.
- Commits em português, Conventional Commits.
- README segue `~/Projetos/Compartilhado/standards/readme.md`: `README.md` (pt-BR) e `README.en-US.md` sempre equivalentes; atualizar quando funcionalidades mudarem.
- Licença MIT só para o código; `resources/` tem origem desconhecida e não pode receber arquivos extraídos de outros jogos.
- Ao concluir trabalho, atualize `REPORT.md` e `HANDOFF.md`. Este arquivo é a fonte única das instruções para agentes; `CLAUDE.md` apenas o importa.
