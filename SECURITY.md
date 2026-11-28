# Política de segurança — Flappy Bird Cover

## Versões suportadas

| Versão | Situação |
| :--- | :--- |
| `main` | Correções aplicadas na branch principal |
| Anteriores | Sem suporte |

## Como relatar uma vulnerabilidade

**Não abra issue pública** com detalhes de exploração.

1. Use **Report a vulnerability** (relato privado) no repositório do GitHub, se o recurso estiver habilitado.
2. Se não estiver, procure o autor ([MAINTAINERS.md](MAINTAINERS.md)) por um canal privado já estabelecido. Não há e-mail de segurança oficial publicado.

Inclua: versão ou commit, plataforma, passos de reprodução e impacto. Não há SLA de resposta nem programa de recompensa.

## Escopo

O executável, o carregamento de arquivos de dados e de salvamento, os scripts de `tools/`, o build e o script `scripts/build-sfml.sh`. Bibliotecas de terceiros só entram quando a falha decorre do uso feito aqui; caso contrário, relate ao projeto de origem.

## Superfície conhecida

- O jogo roda sem privilégios, não usa rede e não lê credenciais; só grava o arquivo de recorde.
- `HighScoreStore` lê um único inteiro; conteúdo inválido vira 0.
- `scripts/build-sfml.sh` baixa o código do SFML 3.0.1 do repositório oficial no GitHub; revise-o antes de executar.
