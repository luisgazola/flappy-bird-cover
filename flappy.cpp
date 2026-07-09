#include "flappy.hpp"

#include <cstdlib>
#include <string>

namespace {
    constexpr unsigned WINDOW_WIDTH = 1000;
    constexpr unsigned WINDOW_HEIGHT = 600;

    constexpr int FPS_LIMIT = 60;

    constexpr float BIRD_SCALE = 2.0f;
    constexpr float PIPE_SCALE = 1.5f;

    constexpr float INITIAL_SPACE = 220.0f;
    constexpr float JUMP_FORCE = -8.0f;
    constexpr float GRAVITY_FORCE = 0.5f;

    constexpr float PIPE_SPEED = -4.0f;
    constexpr int PIPE_SPAWN_INTERVAL = 150;

    constexpr int BIRD_FRAME_WIDTH = 34;
    constexpr int BIRD_FRAME_HEIGHT = 24;
    constexpr float BIRD_ANIMATION_SPEED = 0.15f;
    constexpr int BIRD_TOTAL_FRAMES = 3;
}

FlappyBird::FlappyBird()
{
    window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Flappy Bird Cover",
        sf::Style::Titlebar | sf::Style::Close
    );

    window->setFramerateLimit(FPS_LIMIT);

    gravity = 0.0f;
    frame = 0.0f;
    space = INITIAL_SPACE;
    count = 0;
    score = 0;
    gameover = false;
    add = false;

    bg.loadFromFile("./resources/img/background.png");
    flappy.loadFromFile("./resources/img/flappy.png");
    pipe.loadFromFile("./resources/img/pipe.png");
    font.loadFromFile("./resources/font/flappybird.ttf");

    background = std::make_shared<sf::Sprite>();
    bird = std::make_shared<sf::Sprite>();
    pipeBottom = std::make_shared<sf::Sprite>();
    pipeTop = std::make_shared<sf::Sprite>();

    background->setTexture(bg);

    bird->setTexture(flappy);
    bird->setPosition(
        WINDOW_WIDTH / 2.0f - flappy.getSize().x / 2.0f,
        WINDOW_HEIGHT / 2.0f - flappy.getSize().y / 2.0f
    );
    bird->setScale(BIRD_SCALE, BIRD_SCALE);
    bird->setTextureRect(sf::IntRect(0, 0, BIRD_FRAME_WIDTH, BIRD_FRAME_HEIGHT));

    pipeBottom->setTexture(pipe);
    pipeTop->setTexture(pipe);

    pipeBottom->setScale(PIPE_SCALE, PIPE_SCALE);
    pipeTop->setScale(PIPE_SCALE, -PIPE_SCALE);

    txt_gameover.setFont(font);
    txt_gameover.setString("Press SPACE to restart");
    txt_gameover.setPosition(200.0f, 300.0f);
    txt_gameover.setCharacterSize(50);
    txt_gameover.setOutlineThickness(3);

    txt_score.setFont(font);
    txt_score.setString(std::to_string(score));
    txt_score.setPosition(10.0f, 10.0f);
    txt_score.setCharacterSize(50);
    txt_score.setOutlineThickness(3);
}

void FlappyBird::events()
{
    sf::Event event;

    while (window->pollEvent(event)) {
        const bool isCloseEvent = event.type == sf::Event::Closed;

        const bool isEscapePressed =
            event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape;

        if (isCloseEvent || isEscapePressed) {
            window->close();
        }
    }

    if (gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        score = 0;
        txt_score.setString(std::to_string(score));

        pipes.clear();

        bird->setPosition(
            WINDOW_WIDTH / 2.0f - flappy.getSize().x / 2.0f,
            WINDOW_HEIGHT / 2.0f - flappy.getSize().y / 2.0f
        );

        gravity = 0.0f;
        gameover = false;
    }
}

void FlappyBird::draw()
{
    window->clear(sf::Color::Black);

    window->draw(*background);

    for (const auto& pipeSprite : pipes) {
        window->draw(pipeSprite);
    }

    window->draw(*bird);

    if (gameover) {
        window->draw(txt_gameover);
    }

    window->draw(txt_score);
    window->display();
}

void FlappyBird::run()
{
    while (window->isOpen()) {
        events();
        game();
        draw();

        ++count;

        if (count == 300) {
            count = 0;
        }
    }
}

void FlappyBird::game()
{
    if (!gameover) {
        setAnimeBird();
        moveBird();
        movePipes();
    }
}

void FlappyBird::movePipes()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        gravity = JUMP_FORCE;
        bird->setRotation(-frame - 10.0f);
    } else {
        bird->setRotation(frame - 10.0f);
    }

    if (count % PIPE_SPAWN_INTERVAL == 0) {
        const int pipePosition = std::rand() % 275 + 175;

        pipeBottom->setPosition(WINDOW_WIDTH, pipePosition + space);
        pipeTop->setPosition(WINDOW_WIDTH, pipePosition);

        pipes.push_back(*pipeBottom);
        pipes.push_back(*pipeTop);
    }

    for (std::size_t i = 0; i < pipes.size(); ++i) {
        if (pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds())) {
            bird->move(15.0f, 0.0f);

            if (pipes[i].getScale().y < 0) {
                bird->move(0.0f, -15.0f);
            } else {
                bird->move(0.0f, 15.0f);
            }

            gameover = true;
        }

        if (pipes[i].getPosition().x < -100.0f) {
            pipes.erase(pipes.begin() + i);
            --i;
            continue;
        }

        pipes[i].move(PIPE_SPEED, 0.0f);

        if (static_cast<int>(pipes[i].getPosition().x) == 448 && !add) {
            txt_score.setString(std::to_string(++score));
            add = true;
        } else {
            add = false;
        }
    }
}

void FlappyBird::setAnimeBird()
{
    frame += BIRD_ANIMATION_SPEED;

    if (frame > BIRD_TOTAL_FRAMES) {
        frame -= BIRD_TOTAL_FRAMES;
    }

    bird->setTextureRect(
        sf::IntRect(
            BIRD_FRAME_WIDTH * static_cast<int>(frame),
            0,
            BIRD_FRAME_WIDTH,
            BIRD_FRAME_HEIGHT
        )
    );
}

void FlappyBird::moveBird()
{
    bird->move(0.0f, gravity);
    gravity += GRAVITY_FORCE;
}