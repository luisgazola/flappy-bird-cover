#pragma once

#include "../core/HighScoreStore.hpp"
#include "../core/World.hpp"

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <string>

// Camada de apresentação: janela, entrada, game loop e desenho.
// Toda regra de jogo fica em World.
class Game {
public:
    Game();
    void run();

private:
    void configureSprites();
    void handleEvents();
    void handleKey(sf::Keyboard::Key key);
    void persistBestScore();

    void render();
    void drawPipes();
    void drawBird();
    void drawHud();
    void drawOverlay(std::uint8_t alpha);
    void drawText(const std::string& content, unsigned size,
                  sf::Vector2f position, bool centered);

    // A ordem de declaração importa: no SFML 3 sprites e textos exigem
    // a textura/fonte já carregada no construtor.
    sf::RenderWindow window_;

    sf::Texture backgroundTexture_;
    sf::Texture birdTexture_;
    sf::Texture pipeTexture_;
    sf::Font font_;

    sf::Sprite background_;
    sf::Sprite bird_;
    sf::Sprite pipeTop_;
    sf::Sprite pipeBottom_;
    sf::Text text_;
    sf::RectangleShape overlay_;

    HighScoreStore highScoreStore_;
    int savedBestScore_;
    World world_;

    bool showFps_ = false;
    float smoothedFps_ = 0.0f;
};
