/*********************************************************************


Game Specific : Lamps


*********************************************************************/

#ifndef GS_LAMPS_H
#define GS_LAMPS_H

#include "System/Lamps/LampsHelper.h"

#include <stdint.h>

namespace LampsHelper {
void initLamps();
// void showBonusLamps(uint8_t bonusValue);
void showBonusMultiplierLamps(uint8_t bonusX, uint8_t flashPeriod);
void showTimerLamps(uint8_t baseTimer, uint8_t currentTimer);
} // namespace LampsHelper

//******************************************************************************************************************************************************
//
// Lamps Collections
//
//******************************************************************************************************************************************************

#define MAX_LAMP_EFFECTS 10

#define NUM_LAMP_ANIMATIONS 4
#define LAMP_ANIMATION_STEPS 24
#define NUM_LAMP_ANIMATION_BYTES 8

#define ANIM_ANIMATION1 0
#define ANIM_ANIMATION2 1
#define ANIM_ANIMATION3 2
#define ANIM_ANIMATION4 3

#endif // LAMPS_H
