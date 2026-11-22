#include "World.hpp"

#include "GameConfig.hpp"

#include <algorithm>
#include <cmath>

namespace {
    constexpr float BIRD_START_Y = config::WORLD_HEIGHT / 2.0f;
}

World::World(unsigned randomSeed, int bestScore)
    : bird_(config::BIRD_X, BIRD_START_Y),
      random_(randomSeed),
      bestScore_(std::max(bestScore, 0))
{
}

void World::pressAction()
{
    switch (state_) {
    case GameState::Ready:
        start();
        bird_.flap();
        break;
    case GameState::Playing:
        bird_.flap();
        break;
    case GameState::Paused:
        state_ = GameState::Playing;
        break;
    case GameState::GameOver:
        if (canRestart()) {
            reset();
        }
        break;
    }
}

void World::togglePause()
{
    if (state_ == GameState::Playing) {
        state_ = GameState::Paused;
    } else if (state_ == GameState::Paused) {
        state_ = GameState::Playing;
    }
}

void World::pause()
{
    if (state_ == GameState::Playing) {
        state_ = GameState::Paused;
    }
}

bool World::canRestart() const
{
    return state_ == GameState::GameOver && stateSeconds_ >= config::RESTART_DELAY;
}

float World::currentPipeSpeed() const
{
    return std::min(config::PIPE_INITIAL_SPEED + static_cast<float>(score_) * config::PIPE_SPEED_STEP,
                    config::PIPE_MAX_SPEED);
}

float World::currentPipeGap() const
{
    return std::max(config::PIPE_INITIAL_GAP - static_cast<float>(score_) * config::PIPE_GAP_STEP,
                    config::PIPE_MIN_GAP);
}

void World::update(float deltaSeconds)
{
    if (state_ == GameState::Paused) {
        return;
    }

    elapsedSeconds_ += deltaSeconds;
    stateSeconds_ += deltaSeconds;

    switch (state_) {
    case GameState::Ready:
        updateReady(deltaSeconds);
        break;
    case GameState::Playing:
        updatePlaying(deltaSeconds);
        break;
    case GameState::GameOver:
        updateGameOver(deltaSeconds);
        break;
    case GameState::Paused:
        break;
    }
}

void World::reset()
{
    state_ = GameState::Ready;
    stateSeconds_ = 0.0f;
    score_ = 0;
    isNewBest_ = false;
    pipes_.clear();
    bird_ = Bird(config::BIRD_X, BIRD_START_Y);
}

void World::start()
{
    state_ = GameState::Playing;
    stateSeconds_ = 0.0f;
}

void World::die()
{
    state_ = GameState::GameOver;
    stateSeconds_ = 0.0f;

    if (score_ > bestScore_) {
        bestScore_ = score_;
        isNewBest_ = true;
    }
}

void World::updateReady(float /*deltaSeconds*/)
{
    const float offset = std::sin(elapsedSeconds_ * config::IDLE_BOB_FREQUENCY) *
                         config::IDLE_BOB_AMPLITUDE;
    bird_.setCenterY(BIRD_START_Y + offset);
}

void World::updatePlaying(float deltaSeconds)
{
    bird_.update(deltaSeconds);

    spawnPipesIfNeeded();
    movePipes(deltaSeconds);
    updateScore();

    const bool hitGround = keepBirdInsideWorld();
    const Rect hitbox = bird_.hitbox();
    const bool hitPipe = std::any_of(pipes_.begin(), pipes_.end(),
        [&hitbox](const PipePair& pipe) { return pipe.collidesWith(hitbox); });

    if (hitGround || hitPipe) {
        die();
    }
}

void World::updateGameOver(float deltaSeconds)
{
    // O pássaro continua caindo até o chão; os canos ficam parados.
    bird_.update(deltaSeconds);
    keepBirdInsideWorld();
}

void World::spawnPipesIfNeeded()
{
    float nextLeft = config::WORLD_WIDTH;

    if (!pipes_.empty()) {
        const float lastLeft = pipes_.back().left;
        if (lastLeft > config::WORLD_WIDTH - config::PIPE_SPACING) {
            return;
        }
        nextLeft = lastLeft + config::PIPE_SPACING;
    }

    const float gap = currentPipeGap();
    std::uniform_real_distribution<float> gapTopDistribution(
        config::PIPE_GAP_MARGIN,
        config::WORLD_HEIGHT - gap - config::PIPE_GAP_MARGIN);

    PipePair pipe;
    pipe.left = nextLeft;
    pipe.gapTop = gapTopDistribution(random_);
    pipe.gapHeight = gap;
    pipes_.push_back(pipe);
}

void World::movePipes(float deltaSeconds)
{
    const float distance = currentPipeSpeed() * deltaSeconds;

    for (PipePair& pipe : pipes_) {
        pipe.left -= distance;
    }

    pipes_.erase(
        std::remove_if(pipes_.begin(), pipes_.end(),
            [](const PipePair& pipe) { return pipe.right() < 0.0f; }),
        pipes_.end());
}

void World::updateScore()
{
    for (PipePair& pipe : pipes_) {
        if (!pipe.scored && pipe.right() < bird_.hitbox().left) {
            pipe.scored = true;
            ++score_;
        }
    }
}

// Mantém o pássaro dentro da tela. Retorna true se ele tocou o chão.
bool World::keepBirdInsideWorld()
{
    const float halfHeight = config::BIRD_HEIGHT / 2.0f;

    if (bird_.centerY() - halfHeight < 0.0f) {
        bird_.setCenterY(halfHeight);
        bird_.stop();
    }

    if (bird_.centerY() + halfHeight >= config::WORLD_HEIGHT) {
        bird_.setCenterY(config::WORLD_HEIGHT - halfHeight);
        bird_.stop();
        return true;
    }

    return false;
}
