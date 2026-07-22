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
        break;
    case SW_LEFT_SLING:
    case SW_RIGHT_SLING:
        SoundHelper::playSoundEffect(DASH51_SLINGS, AUDIO_DASH51);
        Scoring::addToStack(STACK_TENS, 1);
        break;
    default:
        break;
    }
}

void UnstructuredPlay::updateLamps() {
    if (!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
