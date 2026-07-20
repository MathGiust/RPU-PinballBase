/*********************************************************************


    Gameplay Header File
    Version : 1.50


*********************************************************************/

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "GameSpecific/PlayerState.h"
#include "PinballMachineBase/Switches.h"
#include "System/MachineState.h"

#include <Arduino.h>

namespace Gameplay {

static constexpr uint8_t playfieldExclusionSwitches[] = {
        SW_OUTHOLE,
        SW_TILT,
        SW_SLAM,
        SW_COIN_1,
        SW_COIN_2,
        SW_COIN_3,
        SW_CREDIT_BUTTON,
};

void init(MachineState* _machineState, PlayerState* _playerStates);
bool run();
void manageNewState();
void handleSwitchHit(uint8_t switchHit);
} // namespace Gameplay
#endif
