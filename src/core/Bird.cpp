#include "Bird.hpp"

#include "GameConfig.hpp"

#include <algorithm>

namespace {
    constexpr float MAX_UP_ROTATION = -25.0f;
    constexpr float MAX_DOWN_ROTATION = 70.0f;
    constexpr float VELOCITY_TO_DEGREES = 1.0f / 12.0f;
}

Bird::Bird(float centerX, float centerY)
    : centerX_(centerX), centerY_(centerY)
{
}

void Bird::flap()
{
    velocity_ = config::FLAP_VELOCITY;
}

void Bird::update(float deltaSeconds)
{
    velocity_ = std::min(velocity_ + config::GRAVITY * deltaSeconds,
                         config::MAX_FALL_VELOCITY);
    centerY_ += velocity_ * deltaSeconds;
}

void Bird::setCenterY(float centerY)
{
    centerY_ = centerY;
}

void Bird::stop()
{
    velocity_ = 0.0f;
}

float Bird::rotationDegrees() const
{
    return std::clamp(velocity_ * VELOCITY_TO_DEGREES,
                      MAX_UP_ROTATION, MAX_DOWN_ROTATION);
}

Rect Bird::hitbox() const
{
    const float inset = config::BIRD_HITBOX_INSET;

    return Rect{
        centerX_ - config::BIRD_WIDTH / 2.0f + inset,
        centerY_ - config::BIRD_HEIGHT / 2.0f + inset,
        config::BIRD_WIDTH - 2.0f * inset,
        config::BIRD_HEIGHT - 2.0f * inset,
    };
}
