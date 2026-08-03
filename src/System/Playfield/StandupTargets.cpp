#include "StandupTargets.h"

StandupTargetBank::StandupTargetBank(const uint8_t _numberOfTargets, const uint8_t* const _switchArray)
    : numberOfTargets(_numberOfTargets),
      switchArray(_switchArray) {}

uint8_t StandupTargetBank::getTargetHitIndex(const uint8_t switchHit) const {
    for (uint8_t i = 0; i < numberOfTargets; i++) {
        if (switchHit == switchArray[i]) return i;
    }
    return 0xFF;
}
