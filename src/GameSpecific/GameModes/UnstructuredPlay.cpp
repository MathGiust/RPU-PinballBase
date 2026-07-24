/*********************************************************************


Unstructured play
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"
#include "GameSpecific/Lamps.h"
#include "GameSpecific/PinballMachineBase/Adjustments.h"
#include "GameSpecific/PinballMachineBase/Lamps.h"
#include "GameSpecific/PinballMachineBase/Playfield.h"
#include "GameSpecific/PinballMachineBase/Solenoids.h"
#include "GameSpecific/PinballMachineBase/Sound.h"
#include "System/Display.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Scoring.h"
#include "System/Sound/Sound.h"
#include "System/Sound/WaveTriggerHandler.h"
#include "System/Utilities.h"

static bool         eyeLit              = false;
static bool         spinnerLit          = false;
static uint8_t      captiveBallProgress = 0;
static Time::time_t lastCaptiveBallTime = 0;

void UnstructuredPlay::onStart() {
    lampsNeedUpdate = true;

    eyeLit              = false;
    spinnerLit          = false;
    captiveBallProgress = 0;
}
void UnstructuredPlay::update() {
    updateLamps();

    if (lastCaptiveBallTime && Time::getCurrentTime() - lastCaptiveBallTime > GS_EEPROM::captiveBallTimer) {
        lampsNeedUpdate = true;
        if (captiveBallProgress) {
            captiveBallProgress--;
            lastCaptiveBallTime = Time::getCurrentTime();
        } else lastCaptiveBallTime = 0;
    }
}
void UnstructuredPlay::onEnd() {
}
void UnstructuredPlay::handleSwitchHit(const uint8_t switchHit) {
    lampsNeedUpdate = true;
    switch (switchHit) {
    case SW_LEFT_BUMPER:
    case SW_RIGHT_BUMPER:
    case SW_BOTTOM_BUMPER:
        currentPlayer->handleBumperHit();
        SoundHelper::playSoundEffect(DASH51_BUMPER, AUDIO_DASH51);
        eyeLit = !eyeLit;
        break;
    case SW_LEFT_SLING:
    case SW_RIGHT_SLING:
        SoundHelper::playSoundEffect(DASH51_SLINGS, AUDIO_DASH51);
        Scoring::addToStack(STACK_TENS, 1);
        eyeLit = !eyeLit;
        if (GS_EEPROM::spinnerDifficulty) spinnerLit = !spinnerLit;
        break;
    case SW_DROP_TARGET_7:
    case SW_DROP_TARGET_4:
    case SW_DROP_TARGET_1:
        if (!machineState->DTB[0]->handleTargetHit(switchHit)) break;
        handleDropTargetHit(switchHit, 0);
        break;
    case SW_DROP_TARGET_2:
    case SW_DROP_TARGET_5:
    case SW_DROP_TARGET_8:
        if (!machineState->DTB[1]->handleTargetHit(switchHit)) break;
        handleDropTargetHit(switchHit, 1);
        break;
    case SW_DROP_TARGET_9:
    case SW_DROP_TARGET_6:
    case SW_DROP_TARGET_3:
        if (!machineState->DTB[2]->handleTargetHit(switchHit)) break;
        handleDropTargetHit(switchHit, 2);
        break;
    case SW_LEFT_SPINNER:
    case SW_RIGHT_SPINNER:
        currentPlayer->handleSpinnerHit();
        break;
    case SW_CAPTIVE_BALL:
        lastCaptiveBallTime = Time::getCurrentTime();
        if (captiveBallProgress == 4) {
            // TODO : Give mystical artifact
            SoundHelper::playSoundEffect(DASH51_SUPER_BONUS, AUDIO_DASH51);
        }
        captiveBallProgress++;
        break;
    default:
        break;
    }
}

void UnstructuredPlay::updateLamps() {
    if (!lampsNeedUpdate) return;
    lampsNeedUpdate = false;

    LampsHelper::setLampState(LAMP_SPINNERS, spinnerLit, 0, 0);

    LampsHelper::setLampCollection(LAMP_COLL_DROP_EYES, eyeLit, 0, 0);
    LampsHelper::setLampCollection(LAMP_COLL_DROP_PYRAMIDS, !eyeLit, 0, 0);

    currentPlayer->showEyeGrid();
    currentPlayer->showPyramidGrid();
    currentPlayer->showCompletedLines();

    // Captive ball
    if (lastCaptiveBallTime) LampsHelper::showOneLampFromCollection(LAMP_COLL_CAPTIVE_BALL, captiveBallProgress, 250);
    else LampsHelper::setLampCollection(LAMP_COLL_CAPTIVE_BALL, false, 0, 0);
}

void UnstructuredPlay::handleDropTargetHit(const uint8_t switchHit, const uint8_t bankNumber) {
    if (machineState->DTB[bankNumber]->checkIfBankCleared()) machineState->DTB[bankNumber]->resetBank(Time::getCurrentTime());
    currentPlayer->handleGridHit(machineState->DTB[bankNumber]->getTargetHitIndex(switchHit), bankNumber, eyeLit);
    SoundHelper::playSoundEffect(eyeLit ? DASH51_EYE : DASH51_PYRAMID, AUDIO_DASH51);
}
