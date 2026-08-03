#include "GameSpecific/Gameplay.h"

#include "GameModes/BaseMode.h"
#include "PinballMachineBase/Solenoids.h"
#include "PlayerState.h"
#include "System/Display.h"
#include "GameModes/GameModes.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Log.h"
#include "System/MachineState.h"
#include "System/RPU.h"
#include "System/Scoring.h"
#include "System/Time.h"
#include "System/Utilities.h"

// Static variables
static MachineState* machineState;
static PlayerState*  playerStates;
static Time::time_t  lastOutholeHitTime = 0;

// Static prototypes
static void updateLamps();
static void updateDisplays();
static void handlePlayfieldValidation(uint8_t switchHit);
static bool manageBallInThrough();
static void manageSystemSwitchHit(uint8_t switchHit);

void Gameplay::init(MachineState* _machineState, PlayerState* _playerStates) {
    machineState = _machineState;
    playerStates = _playerStates;
    GameModes::init(_machineState, _playerStates);
}
void Gameplay::manageNewState() {
    Log::printMessage(Log::LOG_GAMESTATE, "Entering Gameplay");
    while (RPU_PullFirstFromSwitchStack() != SWITCH_STACK_EMPTY) {}

    if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) lastOutholeHitTime = Time::getCurrentTime();

    DisplayHelper::stopAllDisplayOverride();
    DisplayHelper::setBallInPlayDisplayValue(machineState->getBallInPlay());

    LampsHelper::hideAllLamps();

    BaseMode::updateCurrentPlayer(&playerStates[machineState->getCurrentPlayerNumber()]);
    GameModes::startMode(GAMEMODE_UNSTRUCTURED_PLAY);
}

void updateLamps() {
    // Shoot again
    if (machineState->getBallSaveDuration() && Time::getCurrentTime() - machineState->getBallSaveStartTime() < machineState->getBallSaveDuration()) {
        if (Time::getCurrentTime() - machineState->getBallSaveStartTime() > 6000) LampsHelper::setLampState(LAMP_SHOOT_AGAIN_PF, true, 0, 125);
        else LampsHelper::setLampState(LAMP_SHOOT_AGAIN_PF, true, 0, 250);
    } else {
        if (machineState->getSamePlayerShootsAgain()) LampsHelper::setLampState(LAMP_SHOOT_AGAIN_PF, true, 0, 0);
        else LampsHelper::setLampState(LAMP_SHOOT_AGAIN_PF, false, 0, 0);
    }

    // Tilt
    if (machineState->getTilted()) LampsHelper::setLampState(LAMP_TILT, true, 0, 0);
}
void updateDisplays() {
    if (Time::getCurrentTime() - Time::getLastScoreChangeTime() > 1000) {
        if (!machineState->getPlayfieldValidated()) DisplayHelper::setDisplayFlash(machineState->getCurrentPlayerNumber(), true, 500, 0x00);
        else DisplayHelper::setDisplayDash(machineState->getCurrentPlayerNumber(), true, 60);
    } else DisplayHelper::stopDisplayOverride(machineState->getCurrentPlayerNumber());
}
void updateBallSearch() {
    if (Time::getCurrentTime() - Time::getMostRecentSwitchHitTime() > 3000 && RPU_ReadSingleSwitchState(SW_SAUCER)) {
        Time::updateMostRecentSwitchHitTime();
        RPU_PushToTimedSolenoidStack(SOL_SAUCER, SOL_SAUCER_STRENGTH, Time::getCurrentTime());
    }
}
bool manageBallInThrough() {
    if (!lastOutholeHitTime) return false;
    if (!RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
        lastOutholeHitTime = 0;
        return false;
    }
    if (Time::getCurrentTime() - lastOutholeHitTime < 300) return false;
    if (Time::getCurrentTime() - Time::getLastOutholeKickTime() < 300) return false;

    if (!machineState->getPlayfieldValidated()) {
        Log::printMessage(Log::LOG_GAMEPLAY, "Playfield not validated");
        RPU_PushToSolenoidStack(SOL_OUTHOLE, SOL_OUTHOLE_KICKER_STRENGTH, true);
        Time::updateLastOutholeKickTime();
        return false;
    }
    if (Time::getCurrentTime() - machineState->getBallSaveStartTime() < machineState->getBallSaveDuration()) {
        Log::printMessage(Log::LOG_GAMEPLAY, "Ball saved");
        Log::printVariable(Log::LOG_GAMEPLAY, "Ball save Current", Time::getCurrentTime() - machineState->getBallSaveStartTime());
        Log::printVariable(Log::LOG_GAMEPLAY, "Ball save Duration", machineState->getBallSaveDuration());
        machineState->setPlayfieldValidated(false);
        RPU_PushToSolenoidStack(SOL_OUTHOLE, SOL_OUTHOLE_KICKER_STRENGTH, true);
        Time::updateLastOutholeKickTime();
        return false;
    }

    Log::printMessage(Log::LOG_GAMEPLAY, "Ball Over");
    return true;
}
bool Gameplay::run() {
    Scoring::updateScoring();
    GameModes::update();
    updateLamps();
    updateDisplays();
    updateBallSearch();

    return manageBallInThrough();
}

void handlePlayfieldValidation(const uint8_t switchHit) {
    if (!machineState->getPlayfieldValidated() && !Utilities::checkIfByteOnList(Gameplay::playfieldExclusionSwitches, 7, switchHit)) {
        Log::printMessage(Log::LOG_GAMEPLAY, "Playfield validated.");
        machineState->setPlayfieldValidated(true);
        if (!machineState->getBallSaveStartTime()) machineState->setBallSaveStartTime(Time::getCurrentTime());
    }
}
void manageSystemSwitchHit(const uint8_t switchHit) {
    handlePlayfieldValidation(switchHit);

    switch (switchHit) {
    default:
        break;
    case SW_OUTHOLE:
        if (!lastOutholeHitTime) Log::printMessage(Log::LOG_GAMEPLAY, "Trough Activated : Wait for debounce");
        lastOutholeHitTime = Time::getCurrentTime();
        break;
    case SW_TILT:
        machineState->manageTiltWarning();
        break;
    case SW_LEFT_OUTLANE:
    case SW_RIGHT_OUTLANE:
        if (machineState->getBallSaveStartTime()
            && Time::getCurrentTime() - machineState->getBallSaveStartTime() < machineState->getBallSaveDuration()) {
            machineState->setBallSaveStartTime(Time::getCurrentTime());
        }
        break;
    }
}
void Gameplay::handleSwitchHit(const uint8_t switchHit) {
    if (switchHit == MachineState::getMostRecentSwitchHit() && Time::getCurrentTime() - Time::getMostRecentSwitchHitTime() < 30) {
        Log::printVariable(Log::LOG_GAMEPLAY, "Skipped Switch Hit", switchHit);
        return;
    }
    if (machineState->getTilted()) return;

    manageSystemSwitchHit(switchHit);
    GameModes::manageSwitchHit(switchHit);

    Time::updateMostRecentSwitchHitTime();
    MachineState::setMostRecentSwitchHit(switchHit);
}
