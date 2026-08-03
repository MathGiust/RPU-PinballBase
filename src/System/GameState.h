#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <stdint.h>

class GameState {
    GameState (*gameStateFunction)();

public:
    // noinspection CppNonExplicitConvertingConstructor
    // ReSharper disable once CppNonExplicitConvertingConstructor
    GameState(GameState (*_gameStateFunction)())
        : gameStateFunction(_gameStateFunction) {}

    GameState operator()() const {
        return gameStateFunction();
    }
    bool operator==(GameState const& other) const {
        return gameStateFunction == other.gameStateFunction;
    }
    bool operator!=(GameState const& other) const {
        return !operator==(other);
    }

    // Static
    static GameState currentGameState;
    static GameState lastGameState;

    static bool hasChanged() {
        return currentGameState != lastGameState;
    }
};

#endif // GAME_STATE_H
