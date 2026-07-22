/*********************************************************************


Game Specific : Lamps


*********************************************************************/

#ifndef GS_LAMPS_H
#define GS_LAMPS_H

#include <stdint.h>

namespace LampsHelper {
void initLamps();
void showBonusLamps(uint8_t bonusValue);
void showBonusMultiplierLamps(uint8_t bonusX, uint8_t flashPeriod);
void showTimerLamps(uint8_t baseTimer, uint8_t currentTimer);
} // namespace LampsHelper

#endif // GS_LAMPS_H
