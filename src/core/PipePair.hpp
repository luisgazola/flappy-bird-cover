#pragma once

#include "GameConfig.hpp"
#include "Rect.hpp"

// Par de canos (superior e inferior) separados por uma fenda vertical.
struct PipePair {
    float left = 0.0f;
    float gapTop = 0.0f;
    float gapHeight = config::PIPE_INITIAL_GAP;
    bool scored = false;

    float right() const { return left + config::PIPE_WIDTH; }

    Rect topRect() const
    {
        return Rect{left, gapTop - config::PIPE_LENGTH,
                    config::PIPE_WIDTH, config::PIPE_LENGTH};
    }

    Rect bottomRect() const
    {
        return Rect{left, gapTop + gapHeight,
                    config::PIPE_WIDTH, config::PIPE_LENGTH};
    }

    bool collidesWith(const Rect& other) const
    {
        return topRect().intersects(other) || bottomRect().intersects(other);
    }
};
