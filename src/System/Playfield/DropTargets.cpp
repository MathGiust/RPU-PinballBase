#include "DropTargets.h"

#include "GameSpecific/PinballMachineBase/Constants.h"
#include "System/Utilities.h"

DropTargetBank::DropTargetBank(
        const uint8_t  _numberOfTargets,
        const uint8_t* _switchArray,
        const uint8_t  _numberOfResetSolenoids,
        const uint8_t* _resetSolenoidArray,
        const uint8_t  _resetSolenoidStrength,
        bool*          _targetHitArray
) : numberOfTargets(_numberOfTargets),
    numberOfResetSolenoids(_numberOfResetSolenoids),
    resetSolenoidStrength(_resetSolenoidStrength),
    switchArray(_switchArray),
    resetSolenoidArray(_resetSolenoidArray),
    targetHitArray(_targetHitArray),
    bankResetting(false),
    bankResetTime(0),
    targetResetEndTime(0),
    lastResetTime(0) {
}

void DropTargetBank::resetBank(const Time::time_t whenToReset) {
    bool resetTarget = false;
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        if (RPU_ReadSingleSwitchState(switchArray[i])) resetTarget = true;
    }
    if (!resetTarget) return;

    bankResetTime      = whenToReset;
    lastResetTime      = whenToReset;
    targetResetEndTime = whenToReset + (DROP_TARGET_RESET_TIMESTAMP * numberOfResetSolenoids) + DROP_TARGET_RESET_DURATION;
    bankResetting      = true;
}
void DropTargetBank::readBank() const {
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        targetHitArray[i] = RPU_ReadSingleSwitchState(switchArray[i]);
    }
}
void DropTargetBank::update(const Time::time_t currentTime) {
    if (switchArray == nullptr) return;
    if (resetSolenoidArray == nullptr) return;
    if (targetHitArray == nullptr) return;

    if (bankResetTime && currentTime > bankResetTime) { // Time to reset
        bankResetTime = 0;
        for (uint8_t i = 0; i < numberOfResetSolenoids; i++) { // Queue solenoids
            RPU_PushToTimedSolenoidStack(
                    resetSolenoidArray[i], resetSolenoidStrength,
                    Time::getCurrentTime() + i * DROP_TARGET_RESET_TIMESTAMP, true
            );
        }
    } else if (bankResetting && Time::getCurrentTime() > targetResetEndTime) { // Reset is over
        Utilities::resetArray(targetHitArray, numberOfTargets);
        bankResetting      = false;
        targetResetEndTime = 0;
    }
}

bool DropTargetBank::handleTargetHit(const uint8_t targetHit) const {
    if (targetResetEndTime) return false;
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        if (switchArray[i] == targetHit && !targetHitArray[i]) {
            targetHitArray[i] = true;
            return true;
        }
    }
    return false;
}
bool DropTargetBank::checkIfBankCleared() const {
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        if (!targetHitArray[i]) return false;
    }
    return true;
}
bool DropTargetBank::isSwitchHitInBank(const uint8_t switchIndex) const {
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        if (switchArray[i] == switchIndex) return true;
    }
    return false;
}

uint8_t DropTargetBank::getTargetHitIndex(const uint8_t switchHit) const {
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        if (switchArray[i] == switchHit) return i;
    }
    return 0xFF;
}
