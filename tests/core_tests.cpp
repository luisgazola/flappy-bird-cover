// Testes da lógica do jogo (src/core). Não dependem de SFML nem de janela.
#include "../src/core/Bird.hpp"
#include "../src/core/GameConfig.hpp"
#include "../src/core/HighScoreStore.hpp"
#include "../src/core/PipePair.hpp"
#include "../src/core/Rect.hpp"
#include "../src/core/World.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

int failures = 0;

#define CHECK(condition)                                                          \
    do {                                                                          \
        if (!(condition)) {                                                       \
            std::cerr << "  falhou: " #condition " (" << __FILE__ << ":"          \
                      << __LINE__ << ")\n";                                       \
            ++failures;                                                           \
        }                                                                         \
    } while (false)

constexpr float STEP = 1.0f / 120.0f;

void simulate(World& world, float seconds)
{
    for (float elapsed = 0.0f; elapsed < seconds; elapsed += STEP) {
        world.update(STEP);
    }
}

std::filesystem::path temporaryFile(const std::string& name)
{
    return std::filesystem::temp_directory_path() / ("flappy-bird-cover-tests") / name;
}

void testRectIntersection()
{
    const Rect a{0, 0, 10, 10};
    CHECK(a.intersects(Rect{5, 5, 10, 10}));
    CHECK(!a.intersects(Rect{10, 0, 10, 10}));   // apenas encostando
    CHECK(!a.intersects(Rect{20, 20, 5, 5}));
}

void testBirdPhysics()
{
    Bird bird(100.0f, 300.0f);
    bird.flap();
    CHECK(bird.velocity() == config::FLAP_VELOCITY);

    bird.update(0.1f);
    CHECK(bird.centerY() < 300.0f);
    CHECK(bird.velocity() > config::FLAP_VELOCITY);

    for (int i = 0; i < 200; ++i) {
        bird.update(0.05f);
    }
    CHECK(bird.velocity() == config::MAX_FALL_VELOCITY);
    CHECK(bird.rotationDegrees() > 0.0f);
}

void testPipeCollision()
{
    PipePair pipe;
    pipe.left = 100.0f;
    pipe.gapTop = 200.0f;
    pipe.gapHeight = 200.0f;

    CHECK(!pipe.collidesWith(Rect{110, 250, 20, 20}));   // dentro da fenda
    CHECK(pipe.collidesWith(Rect{110, 150, 20, 20}));    // cano de cima
    CHECK(pipe.collidesWith(Rect{110, 450, 20, 20}));    // cano de baixo
    CHECK(!pipe.collidesWith(Rect{300, 150, 20, 20}));   // ao lado
}

void testReadyStateWaitsForAction()
{
    World world(42);
    simulate(world, 5.0f);

    CHECK(world.state() == GameState::Ready);
    CHECK(world.pipes().empty());
    CHECK(std::abs(world.bird().centerY() - config::WORLD_HEIGHT / 2.0f) <=
          config::IDLE_BOB_AMPLITUDE + 0.01f);
}

void testFallingToGroundEndsGame()
{
    World world(42);
    world.pressAction();
    simulate(world, 3.0f);

    CHECK(world.state() == GameState::GameOver);
    CHECK(world.bird().centerY() + config::BIRD_HEIGHT / 2.0f <= config::WORLD_HEIGHT);
}

void testCeilingDoesNotKill()
{
    World world(42);
    world.pressAction();
    for (int i = 0; i < 60; ++i) {
        world.pressAction();
        world.update(STEP);
    }

    CHECK(world.state() == GameState::Playing);
    CHECK(world.bird().centerY() - config::BIRD_HEIGHT / 2.0f >= 0.0f);
}

void testPipesSpawnInsideBoundsWithSpacing()
{
    World world(7);
    world.pressAction();

    for (int round = 0; round < 50; ++round) {
        world.pressAction();
        simulate(world, 0.15f);
        if (world.state() != GameState::Playing) {
            break;
        }
    }

    const auto& pipes = world.pipes();
    CHECK(!pipes.empty());

    for (std::size_t i = 0; i < pipes.size(); ++i) {
        CHECK(pipes[i].gapTop >= config::PIPE_GAP_MARGIN);
        CHECK(pipes[i].gapTop + pipes[i].gapHeight <=
              config::WORLD_HEIGHT - config::PIPE_GAP_MARGIN + 0.01f);
        if (i > 0) {
            CHECK(std::abs(pipes[i].left - pipes[i - 1].left - config::PIPE_SPACING) < 0.01f);
        }
    }
}

// Piloto automático: bate asas quando o pássaro cai abaixo do centro
// da fenda do próximo cano.
void autopilotStep(World& world)
{
    const Bird& bird = world.bird();
    float target = config::WORLD_HEIGHT / 2.0f;

    for (const PipePair& pipe : world.pipes()) {
        if (pipe.right() >= bird.hitbox().left) {
            target = pipe.gapTop + pipe.gapHeight * 0.65f;
            break;
        }
    }

    if (bird.velocity() > 0.0f && bird.centerY() > target) {
        world.pressAction();
    }
}

void testScoreCountsEachPipeOnce()
{
    World world(1);
    world.pressAction();

    int previousScore = 0;
    bool neverJumpedMoreThanOne = true;

    for (float elapsed = 0.0f; elapsed < 20.0f && world.state() == GameState::Playing;
         elapsed += STEP) {
        autopilotStep(world);
        world.update(STEP);
        neverJumpedMoreThanOne &= world.score() - previousScore <= 1;
        previousScore = world.score();
    }

    // 20 s a 240+ px/s com canos a cada 520 px: ~9 canos cruzados.
    CHECK(world.state() == GameState::Playing);
    CHECK(world.score() >= 5);
    CHECK(neverJumpedMoreThanOne);
    CHECK(world.currentPipeSpeed() > config::PIPE_INITIAL_SPEED);
    CHECK(world.currentPipeGap() < config::PIPE_INITIAL_GAP);
}

void testRestartRequiresDelayAndResets()
{
    World fresh(3);
    fresh.pressAction();
    while (fresh.state() == GameState::Playing) {
        fresh.update(STEP);
    }
    fresh.pressAction();
    CHECK(fresh.state() == GameState::GameOver);   // atraso ainda não passou

    simulate(fresh, config::RESTART_DELAY + 0.1f);
    CHECK(fresh.canRestart());
    fresh.pressAction();
    CHECK(fresh.state() == GameState::Ready);
    CHECK(fresh.score() == 0);
    CHECK(fresh.pipes().empty());
}

void testPauseFreezesWorld()
{
    World world(5);
    world.pressAction();
    simulate(world, 0.2f);

    world.togglePause();
    CHECK(world.state() == GameState::Paused);

    const float birdY = world.bird().centerY();
    const float elapsed = world.elapsedSeconds();
    simulate(world, 2.0f);
    CHECK(world.bird().centerY() == birdY);
    CHECK(world.elapsedSeconds() == elapsed);

    world.pressAction();
    CHECK(world.state() == GameState::Playing);

    world.pause();
    world.togglePause();
    CHECK(world.state() == GameState::Playing);
}

void testPauseIgnoredOutsidePlaying()
{
    World world(5);
    world.togglePause();
    CHECK(world.state() == GameState::Ready);
}

void testDifficultyIsBounded()
{
    World world(9);
    CHECK(world.currentPipeGap() == config::PIPE_INITIAL_GAP);
    CHECK(world.currentPipeSpeed() == config::PIPE_INITIAL_SPEED);
    CHECK(config::PIPE_MIN_GAP < config::PIPE_INITIAL_GAP);
    CHECK(config::PIPE_MAX_SPEED > config::PIPE_INITIAL_SPEED);
}

void testBestScoreFromConstructor()
{
    World world(1, 12);
    CHECK(world.bestScore() == 12);
    world.pressAction();
    simulate(world, 3.0f);
    CHECK(world.state() == GameState::GameOver);
    CHECK(world.bestScore() == 12);
    CHECK(!world.isNewBest());

    World negative(1, -5);
    CHECK(negative.bestScore() == 0);
}

void testHighScoreStoreRoundTrip()
{
    const auto path = temporaryFile("nested/highscore.txt");
    std::filesystem::remove_all(path.parent_path());

    HighScoreStore store(path);
    CHECK(store.load() == 0);          // arquivo inexistente
    CHECK(store.save(27));             // cria diretórios
    CHECK(store.load() == 27);

    std::filesystem::remove_all(path.parent_path());
}

void testHighScoreStoreInvalidContent()
{
    const auto path = temporaryFile("invalid.txt");
    std::filesystem::create_directories(path.parent_path());

    {
        std::ofstream file(path);
        file << "abc";
    }
    CHECK(HighScoreStore(path).load() == 0);

    {
        std::ofstream file(path);
        file << "-4";
    }
    CHECK(HighScoreStore(path).load() == 0);

    std::filesystem::remove(path);
}

} // namespace

int main()
{
    const std::vector<std::pair<const char*, std::function<void()>>> tests = {
        {"Rect detecta interseção", testRectIntersection},
        {"Bird aplica impulso, gravidade e velocidade máxima", testBirdPhysics},
        {"PipePair colide só fora da fenda", testPipeCollision},
        {"Ready aguarda ação sem gerar canos", testReadyStateWaitsForAction},
        {"Cair no chão encerra o jogo", testFallingToGroundEndsGame},
        {"Teto limita sem matar", testCeilingDoesNotKill},
        {"Canos nascem dentro da tela e espaçados", testPipesSpawnInsideBoundsWithSpacing},
        {"Pontuação conta cada cano uma vez", testScoreCountsEachPipeOnce},
        {"Reinício respeita atraso e zera o mundo", testRestartRequiresDelayAndResets},
        {"Pausa congela o mundo", testPauseFreezesWorld},
        {"Pausa ignorada fora do jogo", testPauseIgnoredOutsidePlaying},
        {"Dificuldade tem limites", testDifficultyIsBounded},
        {"Recorde inicial é respeitado", testBestScoreFromConstructor},
        {"HighScoreStore grava e lê", testHighScoreStoreRoundTrip},
        {"HighScoreStore ignora conteúdo inválido", testHighScoreStoreInvalidContent},
    };

    for (const auto& [name, test] : tests) {
        const int before = failures;
        test();
        std::cout << (failures == before ? "[ ok ] " : "[FALHA] ") << name << '\n';
    }

    std::cout << '\n' << tests.size() << " testes, " << failures << " verificações falharam\n";
    return failures == 0 ? 0 : 1;
}
