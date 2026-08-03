#ifndef RPU_LAMP_HELPER_H
#define RPU_LAMP_HELPER_H

#include "GameSpecific/PinballMachineBase/Lamps.h"
#include "LampEffect.h"
#include "System/Time.h"

#include <stdint.h>

namespace LampsHelper {
void setLampState(uint8_t lampIndex, bool state, uint8_t dim, uint16_t flashPeriod);
void setLampEffect(
        uint8_t        lampIndex,
        LampEffectType effectType,
        Time::time_t   startTime,
        uint8_t        numberOfCycle,
        uint16_t       period,
        int16_t        acceleration
);
void setFlash(
        uint8_t      lampIndex,
        Time::time_t startTime,
        uint8_t      numberOfCycle,
        uint16_t     period,
        bool         polarity,
        int16_t      acceleration
);
void setGlow(
        uint8_t      lampIndex,
        Time::time_t startTime,
        uint8_t      numberOfCycle,
        uint16_t     period,
        int16_t      acceleration
);

void hideAllLamps();
void hideAllPlayfieldLamps();
void showAllLamps(uint16_t flashPeriod);

uint8_t getCollectionSize(LampCollectionIDs lampCollection);
uint8_t getLampFromCollection(LampCollectionIDs lampCollection, uint8_t lampIndex);
void    setLampFromCollection(LampCollectionIDs lampCollection, uint8_t indexToShow, bool state, uint8_t dim, uint8_t flashPeriod);
void    setLampCollection(LampCollectionIDs lampCollection, bool state, uint8_t dim, uint8_t flashPeriod);
void    flashLitLampsFromCollection(LampCollectionIDs lampCollection, const bool* litLamps, uint8_t flashPeriod, bool offState);
void    showOneLampFromCollection(LampCollectionIDs lampCollection, uint8_t indexToShow, uint8_t flashPeriod = 0);
void    showLampCollectionBetweenIndexes(LampCollectionIDs lampCollection, uint8_t lowerIndex, uint8_t higherIndex);

void stopLampEffect(uint8_t lampIndex);
void stopAllLampEffects();
void stopAllLampAnimations();

void update();
}; // namespace LampsHelper

#endif // RPU_LAMP_HELPER_H
