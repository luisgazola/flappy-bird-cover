#include "Game.hpp"

#include "../core/GameConfig.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>
#include <stdexcept>

namespace {
    constexpr const char* WINDOW_TITLE = "Flappy Bird Cover";
    constexpr const char* ASSETS_DIRECTORY = "resources/";

    constexpr unsigned FPS_LIMIT = 60;

    // Passo fixo de simulação: a física fica igual em qualquer taxa de quadros.
    constexpr float FIXED_TIMESTEP = 1.0f / 120.0f;
    // Evita "espiral da morte" após travamentos (ex.: arrastar a janela).
    constexpr float MAX_FRAME_TIME = 0.25f;

    constexpr float BIRD_SCALE = 2.0f;
    constexpr float PIPE_SCALE = 1.5f;

    constexpr int BIRD_FRAME_WIDTH = 34;
    constexpr int BIRD_FRAME_HEIGHT = 24;
    constexpr int BIRD_TOTAL_FRAMES = 3;
    constexpr float BIRD_FRAMES_PER_SECOND = 9.0f;

    constexpr unsigned TITLE_TEXT_SIZE = 72;
    constexpr unsigned LARGE_TEXT_SIZE = 50;
    constexpr unsigned SMALL_TEXT_SIZE = 28;

    std::string assetPath(const std::string& fileName)
    {
        return std::string(ASSETS_DIRECTORY) + fileName;
    }

    sf::Texture loadTexture(const std::string& fileName)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(assetPath(fileName))) {
            throw std::runtime_error("Falha ao carregar o recurso: " + assetPath(fileName));
        }
        return texture;
    }

    sf::Font loadFont(const std::string& fileName)
    {
        sf::Font font;
        if (!font.openFromFile(assetPath(fileName))) {
            throw std::runtime_error("Falha ao carregar o recurso: " + assetPath(fileName));
        }
        return font;
    }

    sf::IntRect birdFrame(int index)
    {
        return sf::IntRect({index * BIRD_FRAME_WIDTH, 0},
                           {BIRD_FRAME_WIDTH, BIRD_FRAME_HEIGHT});
    }

    bool isActionKey(sf::Keyboard::Key key)
    {
        return key == sf::Keyboard::Key::Space || key == sf::Keyboard::Key::Up ||
               key == sf::Keyboard::Key::W;
    }
}

Game::Game()
    : window_(sf::VideoMode({static_cast<unsigned>(config::WORLD_WIDTH),
                             static_cast<unsigned>(config::WORLD_HEIGHT)}),
              WINDOW_TITLE,
              sf::Style::Titlebar | sf::Style::Close),
      backgroundTexture_(loadTexture("img/background.png")),
      birdTexture_(loadTexture("img/flappy.png")),
      pipeTexture_(loadTexture("img/pipe.png")),
      font_(loadFont("font/flappybird.ttf")),
      background_(backgroundTexture_),
      bird_(birdTexture_, birdFrame(0)),
      pipeTop_(pipeTexture_),
      pipeBottom_(pipeTexture_),
      text_(font_),
      overlay_({config::WORLD_WIDTH, config::WORLD_HEIGHT}),
      highScoreStore_(HighScoreStore::defaultPath()),
      savedBestScore_(highScoreStore_.load()),
      world_(std::random_device{}(), savedBestScore_)
{
    window_.setFramerateLimit(FPS_LIMIT);
    window_.setKeyRepeatEnabled(false);

    configureSprites();
}

void Game::configureSprites()
{
    bird_.setOrigin({BIRD_FRAME_WIDTH / 2.0f, BIRD_FRAME_HEIGHT / 2.0f});
    bird_.setScale({BIRD_SCALE, BIRD_SCALE});

    pipeBottom_.setScale({PIPE_SCALE, PIPE_SCALE});
    // O cano de cima é o mesmo sprite espelhado verticalmente:
    // sua posição y marca a borda inferior.
    pipeTop_.setScale({PIPE_SCALE, -PIPE_SCALE});

    text_.setFillColor(sf::Color::White);
    text_.setOutlineColor(sf::Color::Black);
    text_.setOutlineThickness(3.0f);
}

void Game::run()
{
    sf::Clock clock;
    float accumulator = 0.0f;

    while (window_.isOpen()) {
        const float frameTime = std::min(clock.restart().asSeconds(), MAX_FRAME_TIME);
        accumulator += frameTime;

        if (frameTime > 0.0f) {
            smoothedFps_ = smoothedFps_ * 0.9f + (1.0f / frameTime) * 0.1f;
        }

        handleEvents();

        while (accumulator >= FIXED_TIMESTEP) {
            world_.update(FIXED_TIMESTEP);
            accumulator -= FIXED_TIMESTEP;
        }

        persistBestScore();
        render();
    }
}

void Game::handleEvents()
{
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        } else if (event->is<sf::Event::FocusLost>()) {
            world_.pause();
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                world_.pressAction();
            }
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            handleKey(key->code);
        }
    }
}

void Game::handleKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Escape) {
        window_.close();
    } else if (key == sf::Keyboard::Key::P) {
        world_.togglePause();
    } else if (key == sf::Keyboard::Key::F3) {
        showFps_ = !showFps_;
    } else if (isActionKey(key)) {
        world_.pressAction();
    }
}

void Game::persistBestScore()
{
    if (world_.bestScore() <= savedBestScore_) {
        return;
    }

    savedBestScore_ = world_.bestScore();

    if (!highScoreStore_.save(savedBestScore_)) {
        std::cerr << "Aviso: não foi possível salvar o recorde em "
                  << highScoreStore_.filePath() << '\n';
    }
}

void Game::render()
{
    window_.clear(sf::Color::Black);

    window_.draw(background_);
    drawPipes();
    drawBird();
    drawHud();

    window_.display();
}

void Game::drawPipes()
{
    for (const PipePair& pipe : world_.pipes()) {
        pipeTop_.setPosition({pipe.left, pipe.gapTop});
        pipeBottom_.setPosition({pipe.left, pipe.gapTop + pipe.gapHeight});

        window_.draw(pipeTop_);
        window_.draw(pipeBottom_);
    }
}

void Game::drawBird()
{
    const Bird& bird = world_.bird();

    // Asas param quando o jogo não está rodando.
    const bool animating = world_.state() == GameState::Ready ||
                           world_.state() == GameState::Playing;
    const int frame = animating
        ? static_cast<int>(world_.elapsedSeconds() * BIRD_FRAMES_PER_SECOND) % BIRD_TOTAL_FRAMES
        : 1;

    bird_.setTextureRect(birdFrame(frame));
    bird_.setPosition({bird.centerX(), bird.centerY()});
    bird_.setRotation(sf::degrees(
        world_.state() == GameState::Ready ? 0.0f : bird.rotationDegrees()));

    window_.draw(bird_);
}

void Game::drawHud()
{
    const sf::Vector2f center(config::WORLD_WIDTH / 2.0f, config::WORLD_HEIGHT / 2.0f);

    switch (world_.state()) {
    case GameState::Ready:
        drawText("Flappy Bird Cover", TITLE_TEXT_SIZE, {center.x, 130.0f}, true);
        drawText("Espaco ou clique para jogar", SMALL_TEXT_SIZE,
                 {center.x, center.y + 110.0f}, true);
        drawText("P pausa   F3 FPS   Esc sai", SMALL_TEXT_SIZE,
                 {center.x, center.y + 160.0f}, true);
        if (world_.bestScore() > 0) {
            drawText("Recorde " + std::to_string(world_.bestScore()), SMALL_TEXT_SIZE,
                     {center.x, 210.0f}, true);
        }
        break;

    case GameState::Playing:
        drawText(std::to_string(world_.score()), TITLE_TEXT_SIZE, {center.x, 60.0f}, true);
        break;

    case GameState::Paused:
        drawOverlay(120);
        drawText(std::to_string(world_.score()), TITLE_TEXT_SIZE, {center.x, 60.0f}, true);
        drawText("Pausado", LARGE_TEXT_SIZE, center, true);
        drawText("P ou espaco para continuar", SMALL_TEXT_SIZE,
                 {center.x, center.y + 60.0f}, true);
        break;

    case GameState::GameOver:
        drawOverlay(100);
        drawText("Game Over", TITLE_TEXT_SIZE, {center.x, center.y - 120.0f}, true);
        drawText("Pontos " + std::to_string(world_.score()), LARGE_TEXT_SIZE,
                 {center.x, center.y - 20.0f}, true);
        drawText(world_.isNewBest() ? "Novo recorde!"
                                    : "Recorde " + std::to_string(world_.bestScore()),
                 SMALL_TEXT_SIZE, {center.x, center.y + 40.0f}, true);
        if (world_.canRestart()) {
            drawText("Espaco ou clique para reiniciar", SMALL_TEXT_SIZE,
                     {center.x, center.y + 120.0f}, true);
        }
        break;
    }

    if (showFps_) {
        drawText(std::to_string(static_cast<int>(std::lround(smoothedFps_))) + " FPS",
                 SMALL_TEXT_SIZE, {10.0f, 10.0f}, false);
    }
}

void Game::drawOverlay(std::uint8_t alpha)
{
    overlay_.setFillColor(sf::Color(0, 0, 0, alpha));
    window_.draw(overlay_);
}

void Game::drawText(const std::string& content, unsigned size,
                    sf::Vector2f position, bool centered)
{
    text_.setString(content);
    text_.setCharacterSize(size);

    if (centered) {
        const sf::FloatRect bounds = text_.getLocalBounds();
        text_.setOrigin(bounds.position + bounds.size / 2.0f);
    } else {
        text_.setOrigin({0.0f, 0.0f});
    }

    text_.setPosition(position);
    window_.draw(text_);
}
