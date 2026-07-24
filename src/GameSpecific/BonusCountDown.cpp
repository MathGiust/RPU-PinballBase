#include "GameSpecific/BonusCountDown.h"

#include "GameModes/BaseMode.h"
#include "System/Scoring.h"

static uint8_t grid[3][3]         = {{0}};
static uint8_t completedLineCount = 0;

static void manageNewState(const PlayerState* currentPlayer);
static void updateLamps(PlayerState* currentPlayer);

void manageNewState(const PlayerState* currentPlayer) {
    // Copy grid content
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            grid[i][j] = currentPlayer->getGrid()[i][j];
        }
    }
    completedLineCount = currentPlayer->getCompletedLineCount();
}
void BonusCountDown::run(const boolean curStateChanged, PlayerState* currentPlayer) {
    if (curStateChanged) manageNewState(currentPlayer);

    Scoring::updateScoring();
    updateLamps(currentPlayer);
}

void updateLamps(PlayerState* currentPlayer) {
}
