#include "GameSpecific/PinballMachineBase/Lamps.h"
#include "GameSpecific/RPU_Config.h"
#include "Lamp.h"
#include "LampsHelper.h"
#include "System/RPU.h"
#include "System/Utilities.h"

static Lamp lamps[RPU_MAX_LAMPS];

static bool checkIfCollectionInBound(const LampCollectionIDs lampCollection) {
    return lampCollection >= NUM_LAMP_COLLECTIONS;
}
static bool checkIfLampIndexOutOfCollectionBounds(const LampCollectionIDs lampCollection, const uint8_t lampIndex) {
    return lampIndex >= lampCollections[lampCollection]->getSize();
}

void LampsHelper::setLampState(const uint8_t lampIndex, const bool state, const uint8_t dim, const uint16_t flashPeriod) {
    if (lampIndex >= RPU_MAX_LAMPS) return;

    lamps[lampIndex].state = state;
    lamps[lampIndex].flashPeriod = flashPeriod;
    lamps[lampIndex].dim = dim;
}
void LampsHelper::setLampEffect(
        const uint8_t        lampIndex,
        const LampEffectType effectType,
        Time::time_t         startTime,
        const uint8_t        numberOfCycle,
        const uint16_t       period,
        const int16_t        acceleration
) {
    if (lampIndex >= RPU_MAX_LAMPS) return;
    if (startTime < Time::getCurrentTime()) startTime = Time::getCurrentTime();

    lamps[lampIndex].lampEffect.setLampEffect(
            effectType,
            startTime,
            numberOfCycle,
            period,
            acceleration
    );
}
void LampsHelper::setFlash(
        const uint8_t  lampIndex,
        Time::time_t   startTime,
        const uint8_t  numberOfCycle,
        const uint16_t period,
        const bool     polarity,
        const int16_t  acceleration
) {
    if (lampIndex >= RPU_MAX_LAMPS) return;
    if (startTime < Time::getCurrentTime()) startTime = Time::getCurrentTime();

    // TODO : Duty cycle bounds

    lamps[lampIndex].lampEffect.setFlash(
            startTime,
            numberOfCycle,
            period,
            polarity,
            50,
            acceleration
    );
}
void LampsHelper::setGlow(
        const uint8_t  lampIndex,
        Time::time_t   startTime,
        const uint8_t  numberOfCycle,
        const uint16_t period,
        const int16_t  acceleration
) {
    if (lampIndex >= RPU_MAX_LAMPS) return;
    if (startTime < Time::getCurrentTime()) startTime = Time::getCurrentTime();

    lamps[lampIndex].lampEffect.setGlow(
            startTime,
            numberOfCycle,
            period,
            acceleration
    );
}
void LampsHelper::stopLampEffect(const uint8_t lampIndex) {
    if (lampIndex >= RPU_MAX_LAMPS) return;
    lamps[lampIndex].lampEffect.stop();
}
void LampsHelper::stopAllLampEffects() {
    for (auto& lamp : lamps) {
        lamp.lampEffect.stop();
    }
}

void LampsHelper::hideAllLamps() {
    for (auto& lamp : lamps) {
        lamp.state = false;
    }
}
void LampsHelper::hideAllPlayfieldLamps() {
    for (uint8_t i = 0; i < RPU_MAX_LAMPS; i++) {
        // if (Utilities::checkIfByteOnList())
    }
}
void LampsHelper::showAllLamps(const uint16_t flashPeriod) {
    for (int lampIndex = 0; lampIndex < RPU_MAX_LAMPS; ++lampIndex) {
        setLampState(lampIndex, true, 0, flashPeriod);
    }
}

uint8_t LampsHelper::getCollectionSize(const LampCollectionIDs lampCollection) {
    if (checkIfCollectionInBound(lampCollection)) return 0xFF;
    return lampCollections[lampCollection]->getSize();
}
uint8_t LampsHelper::getLampFromCollection(const LampCollectionIDs lampCollection, const uint8_t lampIndex) {
    if (checkIfCollectionInBound(lampCollection)) return 0xFF;
    if (checkIfLampIndexOutOfCollectionBounds(lampCollection, lampIndex)) return 0xFF;

    return lampCollections[lampCollection]->getLamps()[lampIndex];
}
void LampsHelper::setLampFromCollection(const LampCollectionIDs lampCollection, const uint8_t indexToShow, const bool state, const uint8_t dim, const uint8_t flashPeriod) {
    if (checkIfCollectionInBound(lampCollection)) return;
    if (checkIfLampIndexOutOfCollectionBounds(lampCollection, indexToShow)) return;

    setLampState(getLampFromCollection(lampCollection, indexToShow), state, dim, flashPeriod);
}
void LampsHelper::setLampCollection(const LampCollectionIDs lampCollection, const bool state, const uint8_t dim, const uint8_t flashPeriod) {
    if (checkIfCollectionInBound(lampCollection)) return;

    for (uint8_t i = 0; i < lampCollections[lampCollection]->getSize(); i++) {
        setLampState(getLampFromCollection(lampCollection, i), state, dim, flashPeriod);
    }
}
void LampsHelper::flashLitLampsFromCollection(const LampCollectionIDs lampCollection, const bool* litLamps, const uint8_t flashPeriod, const bool offState) {
    if (checkIfCollectionInBound(lampCollection)) return;
    for (uint8_t i = 0; i < lampCollections[lampCollection]->getSize(); i++) {
        setLampState(getLampFromCollection(lampCollection, i), litLamps[i] ^ offState, 0, flashPeriod);
    }
}
void LampsHelper::showOneLampFromCollection(const LampCollectionIDs lampCollection, const uint8_t indexToShow, const uint8_t flashPeriod) {
    if (checkIfCollectionInBound(lampCollection)) return;
    if (checkIfLampIndexOutOfCollectionBounds(lampCollection, indexToShow)) return;

    for (uint8_t i = 0; i < lampCollections[lampCollection]->getSize(); i++) {
        setLampState(getLampFromCollection(lampCollection, i), i == indexToShow, 0, flashPeriod);
    }
}
void LampsHelper::showLampCollectionBetweenIndexes(const LampCollectionIDs lampCollection, uint8_t lowerIndex, uint8_t higherIndex) {
    const uint8_t collectionSize = lampCollections[lampCollection]->getSize();
    if (lowerIndex >= collectionSize) lowerIndex = collectionSize;
    if (higherIndex >= collectionSize) higherIndex = collectionSize;

    for (uint8_t i = 0; i < collectionSize; i++) {
        if (i >= lowerIndex && i < higherIndex) setLampFromCollection(lampCollection, i, true, 0, 0);
        else setLampFromCollection(lampCollection, i, false, 0, 0);
    }
}

void LampsHelper::stopAllLampAnimations() {
}

static void update(const uint8_t lampIndex, Lamp* const lamp) {
    if (lamp->lampEffect.active) lamp->lampEffect.update(lampIndex);
    else RPU_SetLampState(lampIndex, lamp->state, lamp->dim, lamp->flashPeriod);
}

void LampsHelper::update() {
    for (uint8_t lampIndex = 0; lampIndex < RPU_MAX_LAMPS; lampIndex++) {
        update(lampIndex, &lamps[lampIndex]);
    }

    RPU_ApplyFlashToLamps(Time::getCurrentTime());
}
