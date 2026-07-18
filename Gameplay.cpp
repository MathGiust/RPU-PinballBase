/*********************************************************************


    Gameplay Source File
    Version : 1.50


*********************************************************************/

#include "GameSpecific/Gameplay.h"

#include "GameSpecific/GameModes/GameModes.h"
#include "Lamps.h"
#include "PinballMachineBase/PinballBase.h"
#include "PlayerState.h"
#include "System/Display.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Log.h"
#include "System/MachineState.h"
#include "System/RPU.h"
#include "System/Scoring.h"
#include "System/Sound/Sound.h"
#include "System/Time.h"
#include "System/Utilities.h"

// Static variables
static MachineState* machineState;
static PlayerState*  players;

static bool nextSwitchClosesGate = false;

static void manageNewState(PlayerState* currentPlayer);
static void manageGameplayLights();
static void manageGameplayDisplays();
static int  runGameLoop();
static int  manageBallInThrough();
static int  manageSystemSwitchHit(byte switchHit);

//
//  Public :
//

void Gameplay::initGameplay(MachineState* _machineState, PlayerState* _players) {
    machineState = _machineState;
    players      = _players;
}
int Gameplay::run(const boolean currentStateChanged) {
    if (currentStateChanged) manageNewState(&players[machineState->getCurrentPlayerNumber()]);
    int returnState = runGameLoop();

    // Check if ball is in through
    if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) returnState = manageBallInThrough();
    else Time::resetBallEnteredTroughTime();

    Scoring::updateScoring(); // Update scoring
    manageGameplayLights();   // Update lamps
    manageGameplayDisplays(); // Update displays

    // TODO : Implement specific game routine

    // Check if ball is in saucer
    /*
    if (Time::getCurrentTime() - Time::getMostRecentSwitchHitTime() > 3000 &&
    RPU_ReadSingleSwitchState(SW_SAUCER)) { Time::updateMostRecentSwitchHitTime();
        RPU_PushToTimedSolenoidStack(SOL_SAUCER, SOL_SAUCER_STRENGTH,
    Time::getCurrentTime());
    }
    */

    return returnState;
}

//
//  Private :
//

void manageNewState(PlayerState* currentPlayer) {
    Log::printMessage(Log::LOG_GAMEPLAY, "Entering Game Mode Loop\n");

    while (RPU_PullFirstFromSwitchStack() != SWITCH_STACK_EMPTY)
        ;

    // DISPLAYS
    DisplayHelper::stopAllDisplayOverride();
    DisplayHelper::setBallInPlayDisplayValue(machineState->getBallInPlay());
    /*
    // Display game objective progress :
    if (!machineState->getPlayfieldValidated()) {
        if (Utilities::numberOfTrueElements(currentPlayer->getCompletedObjectives(), NUMBER_OF_OBJECTIVES)) {
            for (uint8_t displayInCheck = 0; displayInCheck < 4; displayInCheck++) {
                if (machineState->getCurrentPlayerNumber() != displayInCheck) {
                    // DisplayHelper::setScoreOverride(displayInCheck, Utilities::getBooleanArrayAsScore(currentPlayer->getCompletedObjectives(), NUMBER_OF_OBJECTIVES), true, NUMBER_OF_OBJECTIVES);
                }
            }
        }
    }
    */

    // SOUND
    /*
    if (machineState->getBallInPlay() != 1) RPU_PlaySoundDash51(DASH51_NEW_BALL);
    else RPU_PlaySoundDash51(DASH51_GAME_START);
    */

    // LAMPS
    LampsHelper::setLampState(LAMP_HIGH_SCORE, false, 0, 0);

    // TIME

    // GAME PLAY
    nextSwitchClosesGate = false;

    // GAME MODE
    GameModes::startMode(GAMEMODE_UNSTRUCTURED_PLAY);
}
int runGameLoop() {
    int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;

    // Update ball save timer :
    if (Time::getCurrentTime() - machineState->getBallSaveTimer() > 10000) {
        machineState->setBallSaveTimer(0);
    }

    // Manage switch hit
    uint8_t switchHit;
    while ((switchHit = RPU_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY) {
        char buf[128];
        returnState = manageSystemSwitchHit(switchHit); // Default scores for switches
        if (switchHit == MachineState::getMostRecentSwitchHit() && (Time::getCurrentTime() - Time::getMostRecentSwitchHitTime() < 30)) {
            Log::printVariable(Log::LOG_GAMEPLAY, "Skipped Switch Hit", switchHit);
        } else {
            Log::printVariable(Log::LOG_GAMEPLAY, "Switch Hit", switchHit);
            if (!machineState->getTilted())
                GameModes::manageSwitchHit(switchHit);
            Time::updateMostRecentSwitchHitTime();
            MachineState::setMostRecentSwitchHit(switchHit);
        }
    }

    GameModes::update();
    return returnState;
}
int manageBallInThrough() {
    int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;

    if (!Time::getBallEnteredTroughTime()) {
        if (DEBUG_MESSAGES)
            Serial.write("Trough Activated\n\r");
        Time::updateBallEnteredTroughTime();
        return MACHINE_STATE_NORMAL_GAMEPLAY;
    }

    // Ball need to settle 750ms to be counted has drained
    if ((Time::getCurrentTime() - Time::getBallEnteredTroughTime()) > 300) {
        if (!machineState->getPlayfieldValidated() || (Time::getCurrentTime() - machineState->getBallSaveTimer() <= MachineState::getEEPROMDataPtr()->ballSaveDuration * 1000)) {
            RPU_PushToTimedSolenoidStack(SOL_OUTHOLE, SOL_OUTHOLE_KICKER_STRENGTH, Time::getCurrentTime());
            machineState->setBallSaveTimer(0);
            Time::resetBallEnteredTroughTime();
            Time::updateLastOutholeKickTime();
            returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
        } else if (Scoring::getAllStacksEmpty() || machineState->getTilted()) { // TODO : TEST getAllStacksEmpty !
            //     LcurrentGameMode->setModeTimer(0);
            // currentGameMode->manageModeTimer(GAME_MODE_UNSTRUCTURED_PLAY);
            // machineState->saveCurrentPlayerProgress();
            returnState = MACHINE_STATE_COUNTDOWN_BONUS;
        }
    }
    return returnState;
}
int manageSystemSwitchHit(const byte switchHit) {
    int          returnState   = MACHINE_STATE_NORMAL_GAMEPLAY;

    if ((!machineState->getPlayfieldValidated()) && !Utilities::checkIfByteOnList(Gameplay::playfieldExclusionSwitches, 7, switchHit)) {
        if (DEBUG_MESSAGES) Serial.println("Playfield validated.");
        machineState->setPlayfieldValidated(true);
        machineState->setBallSaveTimer(Time::getCurrentTime());
    }

    switch (switchHit) {
    // TODO : Default scoring logic
    default:
        break;
    case SW_TILT:
        machineState->manageTiltWarning();
        if (machineState->getTilted()) {
        } else if (Time::getCurrentTime() - Time::getLastTiltWarningTime() > TILT_WARNING_DEBOUNCE_TIME)
            ;
        break;
    case (SW_CREDIT_BUTTON):
        return machineState->manageCreditButton(MACHINE_STATE_NORMAL_GAMEPLAY);
    case SW_COIN_1:
    case SW_COIN_2:
    case SW_COIN_3:
        machineState->manageCoinDrop(switchHit);
        break;
    case SW_SELF_TEST_SWITCH:
        return MACHINE_STATE_TEST_LAMPS;
    }
    return returnState;
}

void manageGameplayDisplays() {
    // Display Dash/Flash
    if (Time::getCurrentTime() - Time::getLastScoreChangeTime() > 1000) {
        if (!machineState->getPlayfieldValidated())
            DisplayHelper::setDisplayFlash(machineState->getCurrentPlayerNumber(), true, 500, 0x00);
        else
            DisplayHelper::setDisplayDash(machineState->getCurrentPlayerNumber(), true, 60);
    } else
        DisplayHelper::stopDisplayOverride(machineState->getCurrentPlayerNumber());
}

// TODO Implement lights of specific game
void manageGameplayLights() {
    // machineState->manageTiltLights();
    // machineState->manageShootAgainLights();
    // LampsHelper::showBonusMultiplierLamps(machineState->getCurrentPlayer()->getBonusMultiplier(), 0);
}

void Gameplay::closeGate() { nextSwitchClosesGate = true; }
