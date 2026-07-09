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
- SFML
- GNU Make
- GCC

---

## Estrutura

```
.
├── flappy.cpp
├── flappy.hpp
├── main.cpp
├── Makefile
├── resources
│   ├── img
│   └── font
└── README.md
```

---

## Compilação

### Instalar dependências

Void Linux

```bash
sudo xbps-install -S gcc make SFML pkg-config
```

Ubuntu

```bash
sudo apt install build-essential libsfml-dev
```

Arch

```bash
sudo pacman -S gcc make sfml
```

---

## Compilar

```bash
make
```

Executar

```bash
make run
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
- [ ] Menu Inicial
- [ ] FPS Counter

---

### Versão 1.2

- [ ] Sistema de Estados
- [ ] Classe Bird
- [ ] Classe Pipe
- [ ] Classe Collision

---

### Versão 2.0

- [ ] Resource Manager
- [ ] Animation Manager
- [ ] UI Manager
- [ ] Save Score
- [ ] Configuração JSON

---

## Autor

Luís Gazola

GitHub

https://github.com/luisgazola

---

## Licença

Projeto destinado exclusivamente para estudos.