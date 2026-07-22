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

static bool eyeLit     = false;
static bool spinnerLit = false;

void UnstructuredPlay::onStart() {
    lampsNeedUpdate = true;

    eyeLit     = false;
    spinnerLit = false;
}
void UnstructuredPlay::update() {
    updateLamps();
}
void UnstructuredPlay::onEnd() {
}
void UnstructuredPlay::handleSwitchHit(const uint8_t switchHit) {
    lampsNeedUpdate = true;
    switch (switchHit) {
    case SW_LEFT_BUMPER:
    case SW_RIGHT_BUMPER:
    case SW_BOTTOM_BUMPER:
        SoundHelper::playSoundEffect(DASH51_BUMPER, AUDIO_DASH51);
        Scoring::addToStack(STACK_HUNDRED, 1);
        eyeLit = !eyeLit;
        if (GS_EEPROM::spinnerDifficulty) spinnerLit = !spinnerLit;
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
        Scoring::addScoreToStacks(spinnerLit ? 1000 : 10);
        SoundHelper::playSoundEffect(spinnerLit ? DASH51_LIT_SPINNER : DASH51_UNLIT_SPINNER, AUDIO_DASH51);
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
}

void UnstructuredPlay::handleDropTargetHit(const uint8_t switchHit, const uint8_t bankNumber) {
    if (machineState->DTB[bankNumber]->checkIfBankCleared()) machineState->DTB[bankNumber]->resetBank(Time::getCurrentTime());
    currentPlayer->handleGridHit(machineState->DTB[bankNumber]->getTargetHitIndex(switchHit), bankNumber, eyeLit);
    SoundHelper::playSoundEffect(eyeLit ? DASH51_EYE : DASH51_PYRAMID, AUDIO_DASH51);
}
