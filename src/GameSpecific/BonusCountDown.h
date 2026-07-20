/*********************************************************************


    Bonus Count Header File
    Version : 1.00


*********************************************************************/

#ifndef BONUS_H
#define BONUS_H

#include "System/MachineState.h"

#include <Arduino.h>

namespace BonusCountDown {
void run(boolean curStateChanged, MachineState& machineState);
}

#endif
