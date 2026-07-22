/*********************************************************************


Unstructured play
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"
#include "GameSpecific/Lamps.h"
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

static bool eyeLit = false;

void UnstructuredPlay::onStart() {
    lampsNeedUpdate = true;
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
        break;
    case SW_LEFT_SLING:
    case SW_RIGHT_SLING:
        SoundHelper::playSoundEffect(DASH51_SLINGS, AUDIO_DASH51);
        Scoring::addToStack(STACK_TENS, 1);
        eyeLit = !eyeLit;
        break;
    case SW_LEFT_DROP_TARGET_1:
    case SW_LEFT_DROP_TARGET_2:
    case SW_LEFT_DROP_TARGET_3:
        if (!machineState->DTB[0]->handleTargetHit(switchHit)) break;
        if (machineState->DTB[0]->checkIfBankCleared()) machineState->DTB[0]->resetBank(Time::getCurrentTime());
        currentPlayer->handleGridHit(0, machineState->DTB[0]->getTargetHitIndex(switchHit), eyeLit);
        break;
    case SW_CENTER_DROP_TARGET_1:
    case SW_CENTER_DROP_TARGET_2:
    case SW_CENTER_DROP_TARGET_3:
        if (!machineState->DTB[1]->handleTargetHit(switchHit)) break;
        if (machineState->DTB[1]->checkIfBankCleared()) machineState->DTB[1]->resetBank(Time::getCurrentTime());
        currentPlayer->handleGridHit(1, machineState->DTB[1]->getTargetHitIndex(switchHit), eyeLit);
        break;
    case SW_RIGHT_DROP_TARGET_1:
    case SW_RIGHT_DROP_TARGET_2:
    case SW_RIGHT_DROP_TARGET_3:
        if (!machineState->DTB[2]->handleTargetHit(switchHit)) break;
        if (machineState->DTB[2]->checkIfBankCleared()) machineState->DTB[2]->resetBank(Time::getCurrentTime());
        currentPlayer->handleGridHit(2, machineState->DTB[2]->getTargetHitIndex(switchHit), eyeLit);
        break;
    default:
        break;
    }
}

void UnstructuredPlay::updateLamps() {
    if (!lampsNeedUpdate) return;
    lampsNeedUpdate = false;

    LampsHelper::setLampCollection(LAMP_COLL_DROP_EYES, eyeLit, 0, 0);
    LampsHelper::setLampCollection(LAMP_COLL_DROP_PYRAMIDS, !eyeLit, 0, 0);

    currentPlayer->showEyeGrid();
    currentPlayer->showPyramidGrid();
}
