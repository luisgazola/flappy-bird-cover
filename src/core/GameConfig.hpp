#pragma once

// Parâmetros do jogo. Distâncias em pixels, tempo em segundos,
// velocidades em px/s e acelerações em px/s².
namespace config {

inline constexpr float WORLD_WIDTH = 1000.0f;
inline constexpr float WORLD_HEIGHT = 600.0f;

// Pássaro (sprite de 34x24 escalado 2x). A posição é o centro do sprite.
inline constexpr float BIRD_X = 280.0f;
inline constexpr float BIRD_WIDTH = 68.0f;
inline constexpr float BIRD_HEIGHT = 48.0f;
inline constexpr float BIRD_HITBOX_INSET = 8.0f;

inline constexpr float GRAVITY = 1800.0f;
inline constexpr float FLAP_VELOCITY = -480.0f;
inline constexpr float MAX_FALL_VELOCITY = 900.0f;

// Oscilação do pássaro na tela inicial.
inline constexpr float IDLE_BOB_AMPLITUDE = 10.0f;
inline constexpr float IDLE_BOB_FREQUENCY = 4.0f;

// Canos (sprite de 52x320 escalado 1.5x).
inline constexpr float PIPE_WIDTH = 78.0f;
inline constexpr float PIPE_LENGTH = 480.0f;
inline constexpr float PIPE_SPACING = 520.0f;
inline constexpr float PIPE_GAP_MARGIN = 60.0f;

// Dificuldade progressiva: a cada ponto a fenda diminui e a velocidade sobe.
inline constexpr float PIPE_INITIAL_GAP = 220.0f;
inline constexpr float PIPE_MIN_GAP = 165.0f;
inline constexpr float PIPE_GAP_STEP = 2.0f;

inline constexpr float PIPE_INITIAL_SPEED = 240.0f;
inline constexpr float PIPE_MAX_SPEED = 380.0f;
inline constexpr float PIPE_SPEED_STEP = 4.0f;

// Tempo mínimo na tela de game over antes de aceitar reinício,
// evitando reinícios acidentais por um toque que ainda estava em andamento.
inline constexpr float RESTART_DELAY = 0.6f;

} // namespace config
