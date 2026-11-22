#pragma once

#include "Bird.hpp"
#include "PipePair.hpp"

#include <random>
#include <vector>

enum class GameState {
    Ready,     // tela inicial, aguardando o primeiro toque
    Playing,
    Paused,
    GameOver,
};

// Regras do jogo: estados, física, canos, colisão e pontuação.
// Não depende de SFML, o que permite testá-la isoladamente.
class World {
public:
    explicit World(unsigned randomSeed, int bestScore = 0);

    // Ação principal (espaço/clique): inicia, bate as asas ou reinicia,
    // conforme o estado atual.
    void pressAction();

    void togglePause();
    void pause();

    void update(float deltaSeconds);

    GameState state() const { return state_; }
    int score() const { return score_; }
    int bestScore() const { return bestScore_; }
    bool isNewBest() const { return isNewBest_; }
    bool canRestart() const;

    const Bird& bird() const { return bird_; }
    const std::vector<PipePair>& pipes() const { return pipes_; }

    // Tempo acumulado desde a criação, usado por animações.
    float elapsedSeconds() const { return elapsedSeconds_; }

    float currentPipeSpeed() const;
    float currentPipeGap() const;

private:
    void reset();
    void start();
    void die();

    void updateReady(float deltaSeconds);
    void updatePlaying(float deltaSeconds);
    void updateGameOver(float deltaSeconds);

    void spawnPipesIfNeeded();
    void movePipes(float deltaSeconds);
    void updateScore();
    bool keepBirdInsideWorld();

    GameState state_ = GameState::Ready;
    Bird bird_;
    std::vector<PipePair> pipes_;
    std::mt19937 random_;

    int score_ = 0;
    int bestScore_ = 0;
    bool isNewBest_ = false;

    float elapsedSeconds_ = 0.0f;
    float stateSeconds_ = 0.0f;
};
