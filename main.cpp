#include "flappy.hpp"

#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    FlappyBird game;
    game.run();

    return 0;
}