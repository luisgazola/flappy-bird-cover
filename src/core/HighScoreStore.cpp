#include "HighScoreStore.hpp"

#include <cstdlib>
#include <fstream>
#include <system_error>
#include <utility>

namespace {
    constexpr const char* APP_DIRECTORY = "flappy-bird-cover";
    constexpr const char* FILE_NAME = "highscore.txt";
}

HighScoreStore::HighScoreStore(std::filesystem::path filePath)
    : filePath_(std::move(filePath))
{
}

int HighScoreStore::load() const
{
    std::ifstream file(filePath_);
    int score = 0;

    if (!(file >> score) || score < 0) {
        return 0;
    }

    return score;
}

bool HighScoreStore::save(int score) const
{
    std::error_code error;

    if (filePath_.has_parent_path()) {
        std::filesystem::create_directories(filePath_.parent_path(), error);
        if (error) {
            return false;
        }
    }

    std::ofstream file(filePath_, std::ios::trunc);
    file << score << '\n';

    return static_cast<bool>(file);
}

std::filesystem::path HighScoreStore::defaultPath()
{
    if (const char* dataHome = std::getenv("XDG_DATA_HOME"); dataHome && *dataHome) {
        return std::filesystem::path(dataHome) / APP_DIRECTORY / FILE_NAME;
    }

    if (const char* home = std::getenv("HOME"); home && *home) {
        return std::filesystem::path(home) / ".local" / "share" / APP_DIRECTORY / FILE_NAME;
    }

    return FILE_NAME;
}
