#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

class FlappyBird {
private:
    std::shared_ptr<sf::RenderWindow> window;

    float gravity;
    float frame;
    float space;

    int count;
    int score;

    bool gameover;
    bool add;

    sf::Texture bg;
    sf::Texture flappy;
    sf::Texture pipe;

    std::shared_ptr<sf::Sprite> background;
    std::shared_ptr<sf::Sprite> bird;
    std::shared_ptr<sf::Sprite> pipeBottom;
    std::shared_ptr<sf::Sprite> pipeTop;

    std::vector<sf::Sprite> pipes;

    sf::Font font;
    sf::Text txt_score;
    sf::Text txt_gameover;

private:
    void events();
    void draw();
    void game();

    void movePipes();
    void moveBird();
    void setAnimeBird();

public:
    FlappyBird();
    void run();
};