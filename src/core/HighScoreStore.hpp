#pragma once

#include <filesystem>

// Persiste o recorde em um arquivo de texto com um único número.
class HighScoreStore {
public:
    explicit HighScoreStore(std::filesystem::path filePath);

    // Retorna 0 se o arquivo não existir ou for inválido.
    int load() const;

    // Retorna false se não conseguir gravar (o jogo segue sem persistir).
    bool save(int score) const;

    const std::filesystem::path& filePath() const { return filePath_; }

    // $XDG_DATA_HOME/flappy-bird-cover/highscore.txt,
    // ou ~/.local/share/flappy-bird-cover/highscore.txt,
    // ou ./highscore.txt se nenhuma das variáveis existir.
    static std::filesystem::path defaultPath();

private:
    std::filesystem::path filePath_;
};
