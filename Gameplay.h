/*********************************************************************


    Gameplay Header File
    Version : 1.50


*********************************************************************/

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "GameSpecific/PinballMachineBase/Switches.h"
#include "PlayerState.h"
#include "System/MachineState.h"

#include <Arduino.h>

namespace Gameplay {
void initGameplay(MachineState* _machineState, PlayerState* _players);
int  run(boolean currentStateChanged);
void setGameMode(byte newGameMode);
void closeGate();

static constexpr uint8_t playfieldExclusionSwitches[] = {
        SW_OUTHOLE,
        SW_TILT,
        SW_SLAM,
        SW_COIN_1,
        SW_COIN_2,
        SW_COIN_3,
        SW_CREDIT_BUTTON,
};
} // namespace Gameplay
#endif
