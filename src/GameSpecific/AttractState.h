/*********************************************************************


    Attract Header File
    Version : 1.00


*********************************************************************/

#ifndef ATTRACT_STATE_H
#define ATTRACT_STATE_H

#include "Arduino.h"
#include "System/MachineState.h"

#define FEATURE_SHOW_TIMEOUT 60000
#define TAUNT_TIMEOUT 15000

namespace Attract {
void run(boolean currentStateChanged, MachineState& machineState);
void handleSwitchHit(uint8_t switchHit);
} // namespace Attract

#endif
