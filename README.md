# 🐦 Flappy Bird Cover

Um projeto desenvolvido em **C++17** utilizando **SFML** com o objetivo de estudar desenvolvimento de jogos, programação orientada a objetos e arquitetura de software.

---

## Objetivos

Este projeto não pretende apenas reproduzir o Flappy Bird.

Seu principal objetivo é servir como laboratório para aprendizado de:

- C++ Moderno
- SFML
- Programação Orientada a Objetos
- Game Loop
- Física
- Colisão
- Animações
- Estruturação de projetos
- Clean Code
- Git
- GitHub

---

## Tecnologias

- C++17
- SFML 3
- GNU Make
- GCC

---

## Controles

| Tecla / ação | Efeito |
| --- | --- |
| `Espaço`, `↑`, `W` ou clique esquerdo | Inicia, bate as asas ou reinicia |
| `P` | Pausa / continua (perder o foco da janela também pausa) |
| `F3` | Mostra / oculta o contador de FPS |
| `Esc` | Sai do jogo |

O recorde é salvo em `$XDG_DATA_HOME/flappy-bird-cover/highscore.txt` (padrão: `~/.local/share/flappy-bird-cover/highscore.txt`).

---

## Arquitetura

O código é dividido em duas camadas:

- **`src/core/`** — regras do jogo, sem dependência de SFML: física do pássaro (`Bird`), canos (`PipePair`), colisão (`Rect`), estados e pontuação (`World`), persistência do recorde (`HighScoreStore`) e parâmetros (`GameConfig.hpp`). Usa unidades por segundo, então o comportamento não depende da taxa de quadros.
- **`src/app/`** — apresentação com SFML (`Game`): janela, entrada por eventos, game loop com passo fixo de 1/120 s e desenho.

Estados do jogo: `Ready` → `Playing` ⇄ `Paused` → `GameOver` → `Ready`.

Decisão registrada em [docs/adr/0001-separar-nucleo-e-apresentacao.md](docs/adr/0001-separar-nucleo-e-apresentacao.md).

```
.
├── src
│   ├── core          # lógica pura (testável)
│   ├── app           # SFML
│   └── main.cpp
├── tests
│   └── core_tests.cpp
├── resources
│   ├── img
│   └── font
├── docs/adr
├── Makefile
└── README.md
```

---

## Compilação

### Instalar dependências

Void Linux

```bash
sudo xbps-install -S gcc make SFML-devel pkg-config
```

Ubuntu

```bash
sudo apt install build-essential pkg-config libsfml-dev
```

Arch

```bash
sudo pacman -S gcc make pkgconf sfml
```

Gentoo

```bash
sudo emerge --ask media-libs/libsfml
```

O código usa a API do **SFML 3** (validado com 3.0.1 no Gentoo). Algumas distribuições ainda empacotam o SFML 2.x (por exemplo, Ubuntu 24.04), que é incompatível; nesse caso o `make` avisa e é preciso instalar o SFML 3 por outra via.

---

## Compilar

```bash
make
```

Executar (a partir da raiz do projeto, por causa de `resources/`)

```bash
make run
```

Testes da lógica do jogo (não precisam de SFML nem de janela)

```bash
make test
```

Limpar

```bash
make clean
```

---

## Roadmap

### Versão 1.0

- [x] Janela
- [x] Background
- [x] Bird
- [x] Canos
- [x] Pontuação
- [x] Reinício
- [x] Game Over

---

### Versão 1.1

- [ ] Sons
- [ ] Música
- [x] Menu Inicial
- [x] FPS Counter
- [x] Pausa

---

### Versão 1.2

- [x] Sistema de Estados
- [x] Classe Bird
- [x] Classe Pipe (`PipePair`)
- [x] Classe Collision (`Rect`)
- [x] Física independente de FPS (passo fixo)
- [x] Testes automatizados da lógica

---

### Versão 2.0

- [ ] Resource Manager
- [ ] Animation Manager
- [ ] UI Manager
- [x] Save Score
- [ ] Configuração JSON

---

## Autor

Luís Gazola

GitHub

https://github.com/luisgazola

---

## Licença

Projeto destinado exclusivamente para estudos.