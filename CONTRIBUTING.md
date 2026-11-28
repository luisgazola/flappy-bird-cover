# Como contribuir — Flappy Bird Cover

Projeto pessoal de estudo de Luís Fernando Gazola, em repositório público. O código está sob a [licença MIT](LICENSE): ao enviar uma contribuição, você concorda em licenciá-la sob a mesma licença. A aceitação de pull requests fica a critério do autor ([MAINTAINERS.md](MAINTAINERS.md)).

## O que é bem-vindo

- Correções de bugs e testes novos para o núcleo.
- Melhorias de legibilidade e de arquitetura alinhadas ao [ADR 0001](docs/adr/0001-separar-nucleo-e-apresentacao.md).
- **Recursos próprios** (sprites, fonte com acentos, sons) com licença declarada, para substituir os de `resources/`, cuja origem é desconhecida ([NOTICE.md](NOTICE.md)).

Não envie arquivos extraídos do jogo original ou de outros jogos.

## Ambiente

```bash
make            # SFML 3 do sistema
make test       # núcleo, sem SFML
make run
./scripts/build-sfml.sh .deps/sfml && make SFML_PREFIX="$PWD/.deps/sfml"   # sem SFML 3 no sistema
```

## Convenções

- `src/core/` nunca inclui SFML; mudança de comportamento ali exige teste em `tests/core_tests.cpp`.
- Compilação sem avisos com `-Wall -Wextra -Wpedantic -Wshadow -Wconversion`.
- A fonte atual não tem acentos: textos da interface sem acentuação.
- Constantes de jogabilidade só em `src/core/GameConfig.hpp`.
- Decisões em `docs/adr/`.
- Commits em [Conventional Commits](https://www.conventionalcommits.org/pt-br/v1.0.0/), em português.

## Pull requests

1. Crie uma branch a partir de `main`.
2. Rode `make test` e `make` sem avisos.
3. Descreva o que mudou e o que foi testado de fato. A CI (`.github/workflows/ci.yml`) precisa passar.
4. Atualize o README (pt-BR e en-US) quando funcionalidades mudarem, e [REPORT.md](REPORT.md) e [HANDOFF.md](HANDOFF.md) quando o estado mudar.

Participação sujeita ao [Código de conduta](CODE_OF_CONDUCT.md). Vulnerabilidades: [SECURITY.md](SECURITY.md).
