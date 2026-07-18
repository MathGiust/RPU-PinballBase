/*********************************************************************


Game Specific : Lamps
Version : 0.00


*********************************************************************/

#include "GameSpecific/PinballMachineBase/Lamps.h"

#include "GameSpecific/Lamps.h"
#include "System/Lamps/LampCollection.h"

#include <Arduino.h>

static uint8_t descendingBonusValues[] = {
        40, 30, 20,
        10, 9, 8, 7,
        6, 5, 4,
        3, 2,
        1
};

void LampsHelper::showBonusLamps(uint8_t bonusValue) {
    /*
    for (uint8_t itr = 0; itr < getCollectionSize(COLL_BONUS_LAMPS); itr++) {
        if (bonusValue >= descendingBonusValues[itr]) {
            setLampState(getLampFromCollection(COLL_BONUS_LAMPS, itr), true, 0, 0);
            bonusValue -= descendingBonusValues[itr];
        } else {
            setLampState(getLampFromCollection(COLL_BONUS_LAMPS, itr), false, 0, 0);
        }
    }
    */
}

void LampsHelper::showBonusMultiplierLamps(const uint8_t bonusX, const uint8_t flashPeriod) {
    if (bonusX < 2) {
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
        setLampState(LAMP_4X_BONUS, false, 0, 0);
    }
    if (bonusX == 2) {
        setLampState(LAMP_2X_BONUS, true, 0, flashPeriod);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
        setLampState(LAMP_4X_BONUS, false, 0, 0);
    }
    if (bonusX == 3) {
        setLampState(LAMP_3X_BONUS, true, 0, flashPeriod);
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_4X_BONUS, false, 0, 0);
    }
    if (bonusX == 4) {
        setLampState(LAMP_2X_BONUS, false, 0, 0);
        setLampState(LAMP_3X_BONUS, false, 0, 0);
        setLampState(LAMP_4X_BONUS, true, 0, 0);
    }
}
