#pragma once

#include "Rect.hpp"

// Física do pássaro. Não conhece renderização nem entrada.
class Bird {
public:
    Bird(float centerX, float centerY);

    void flap();
    void update(float deltaSeconds);

    void setCenterY(float centerY);
    void stop();

    float centerX() const { return centerX_; }
    float centerY() const { return centerY_; }
    float velocity() const { return velocity_; }

    // Inclinação em graus derivada da velocidade vertical
    // (nariz para cima ao subir, para baixo ao cair).
    float rotationDegrees() const;

    // Caixa de colisão um pouco menor que o sprite, para perdoar
    // as bordas transparentes e a rotação.
    Rect hitbox() const;

private:
    float centerX_;
    float centerY_;
    float velocity_ = 0.0f;
};
