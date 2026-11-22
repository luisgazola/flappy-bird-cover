#pragma once

// Retângulo alinhado aos eixos, independente de biblioteca gráfica.
struct Rect {
    float left = 0.0f;
    float top = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    float right() const { return left + width; }
    float bottom() const { return top + height; }

    bool intersects(const Rect& other) const
    {
        return left < other.right() && other.left < right() &&
               top < other.bottom() && other.top < bottom();
    }
};
