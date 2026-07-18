/*********************************************************************


    Bonus Count Header File
    Version : 1.00


*********************************************************************/

#ifndef BONUS_H
#define BONUS_H

#include "PlayerState.h"
#include "System/MachineState.h"

#include <Arduino.h>

namespace BonusCountDown {
uint8_t run(bool curStateChanged, PlayerState* _currentPlayer);
}

#endif
