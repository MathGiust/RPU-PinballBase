/*********************************************************************


Game Specific : Lamps
Version : 0.00


*********************************************************************/

#include "GameSpecific/PinballMachineBase/Lamps.h"

#include "GameSpecific/Lamps.h"
#include "System/Lamps/LampCollection.h"

#include <Arduino.h>

void LampsHelper::showBonusMultiplierLamps(const uint8_t bonusX, const uint8_t flashPeriod) {
    if (bonusX < 2) {
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
        setLampState(LAMP_5X_BONUS, false, 0, 0);
    }
    if (bonusX == 2) {
        setLampState(LAMP_2X_BONUS, true, 0, flashPeriod);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
        setLampState(LAMP_5X_BONUS, false, 0, 0);
    }
    if (bonusX == 3) {
        setLampState(LAMP_3X_BONUS, true, 0, flashPeriod);
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_5X_BONUS, false, 0, 0);
    }
    if (bonusX == 4) {
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
        setLampState(LAMP_5X_BONUS, false, 0, 0);
    }
    if (bonusX >= 5) {
        setLampState(LAMP_5X_BONUS, true, 0, flashPeriod);
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
    }
}
