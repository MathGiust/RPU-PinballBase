/*********************************************************************


Game Modes Source File
Version : 1.10


*********************************************************************/

#include "GameModes.h"

#include "BaseMode.h"
#include "GameSpecific/PinballMachineBase/PinballBase.h"
#include "System/Time.h"

void GameModes::initGameModes(MachineState* _machineState, PlayerState* _players) {
    BaseMode::initBaseMode(_machineState, _players);
    BaseMode::updateCurrentPlayer();
    BaseMode::setGlobalTimer(0);
}
void GameModes::initNewBall() {
    BaseMode::updateCurrentPlayer();
}

// Game modes Lifes
void GameModes::startMode(const GameModeIDs GameModeID) {
    if (GameModeID < NUMBER_OF_MODES && gameModes[GameModeID]) {
        gameModes[GameModeID]->setActive(true);
        gameModes[GameModeID]->onStart();
    }
}
void GameModes::stopMode(const GameModeIDs GameModeID) {
    if (GameModeID < NUMBER_OF_MODES && gameModes[GameModeID] && gameModes[GameModeID]->isActive()) {
        gameModes[GameModeID]->onEnd();
        gameModes[GameModeID]->setActive(false);
    }
}
void GameModes::update() {
    for (auto& gameMode : gameModes) {
        if (gameMode && gameMode->isActive()) {
            gameMode->incrementTimer();
            gameMode->update();
        }
    }
}
void GameModes::manageSwitchHit(const uint8_t switchHit) {
    for (auto& gameMode : gameModes) {
        if (gameMode && gameMode->isActive())
            gameMode->handleSwitchHit(switchHit);
    }
}

// Getters
bool GameModes::getGameModeStatus(const GameModeIDs GameModeID) {
    if (GameModeID < NUMBER_OF_MODES && gameModes[GameModeID]) return gameModes[GameModeID]->isActive();
    return false;
}

// Game mode Timer
void GameModes::manageModeTimer() {
    if (Time::get1000msIT()) {
        if (getGlobalTimer()) setGlobalTimer(getGlobalTimer() - 1); // Decrease mode timer
#ifdef DEBUG_MESSAGES
        Serial.print("Mode timer : ");
        Serial.println(getGlobalTimer());
#endif
    }
    for (auto& gameMode : gameModes) {
        if (gameMode->isActive() && !gameMode->isGlobalTimerUsed() && gameMode->getTimer()) {
            gameMode->setTimer(gameMode->getTimer());
        }
    }
}
uint8_t GameModes::getGlobalTimer() {
    return BaseMode::getGlobalTimer();
}
void GameModes::setGlobalTimer(const uint8_t value) {
    BaseMode::setGlobalTimer(value);
}
