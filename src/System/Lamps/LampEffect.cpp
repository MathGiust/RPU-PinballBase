#include "LampEffect.h"

#include "GameSpecific/PinballMachineBase/Constants.h"
#include "GameSpecific/RPU_Config.h"
#include "System/RPU.h"

void LampEffect::setLampEffect(
        const LampEffectType effect,
        const Time::time_t   startTime,
        const uint8_t        numberOfCycle,
        const uint16_t       period,
        const int16_t        acceleration
) {
    type = effect;
    effectStartTime = startTime;
    cycleRemaining = numberOfCycle;
    cyclePeriod = period;
    cycleAcceleration = acceleration;
    active = true;
}

void LampEffect::setFlash(
        const Time::time_t startTime,
        const uint8_t      numberOfCycle,
        const uint16_t     period,
        const bool         polarity,
        const uint8_t      dutyCycle,
        const int16_t      acceleration
) {
    if (cycleRemaining) return;

    setLampEffect(
            LampEffectType::FLASH,
            startTime,
            numberOfCycle,
            period,
            acceleration
    );
    flashPolarity = polarity;
    flashDutyCycle = dutyCycle;
    nextCycleStepTime = startTime + cyclePeriod / 2;
}

void LampEffect::setGlow(
        const Time::time_t startTime,
        const uint8_t      numberOfCycle,
        const uint16_t     period,
        const int16_t      acceleration
) {
    setLampEffect(
            LampEffectType::GLOW,
            startTime,
            numberOfCycle,
            period,
            acceleration
    );

    nextCycleStepTime = startTime + cyclePeriod / 16;
}
void LampEffect::stop() {
    active = false;
    cycleRemaining = 0;
}

void LampEffect::decreaseNumberOfCycleRemaining() {
    if (cycleRemaining == 0xFF) return;

    if (cycleRemaining) cycleRemaining--;
    else active = false;
}
void LampEffect::computeNextPeriodDuration() {
    int32_t newPeriod = static_cast<int32_t>(cyclePeriod) + cycleAcceleration;
    if (newPeriod < LAMP_FLASH_MIN_PERIOD) newPeriod = LAMP_FLASH_MIN_PERIOD;
    cyclePeriod = static_cast<uint16_t>(newPeriod);
}
void LampEffect::checkIfCycleCompleted(const uint8_t numberOfSteps) {
    if (cycleStep >= numberOfSteps) {
        cycleStep = 0;
        decreaseNumberOfCycleRemaining();
        computeNextPeriodDuration();
    } else cycleStep++;
}

void LampEffect::computeNextFlashStepTime() {
    // TODO : Duty cycle
    // if (cycleStep) nextCycleStepTime = cyclePeriod - cyclePeriod / flashDutyCycle;
    // else nextCycleStepTime = cyclePeriod / flashDutyCycle;
    // nextCycleStepTime += Time::getCurrentTime();
    nextCycleStepTime = (cyclePeriod / 2) + Time::getCurrentTime();

    Serial.println(Time::getCurrentTime());
    Serial.println(nextCycleStepTime);
}
void LampEffect::updateFlash(const uint8_t lampIndex) {
    if (Time::getCurrentTime() < nextCycleStepTime) return;

    RPU_SetLampState(lampIndex, cycleStep ^ flashPolarity, 0, 0);

    checkIfCycleCompleted(1);
    computeNextFlashStepTime();
}

void LampEffect::computeNextGlowStepTime() {
    nextCycleStepTime = Time::getCurrentTime() + cyclePeriod / 16;
}
void LampEffect::updateGlow(const uint8_t lampIndex) {
    static constexpr int8_t glowTable[16] = {
            0, 0, 0, 1,
            2, -1, -1, -1,
            -1, -1, -1, -1,
            2, 1, 0, 0
    };

    if (Time::getCurrentTime() < nextCycleStepTime) return;

    const int level = glowTable[cycleStep];
    RPU_SetLampState(lampIndex, level != -1, abs(level), 0);

    checkIfCycleCompleted(15);
    computeNextGlowStepTime();
}

void LampEffect::update(const uint8_t lampIndex) {
    if (lampIndex >= RPU_MAX_LAMPS) return;
    if (!active) return;
    if (Time::getCurrentTime() < effectStartTime) return;

    switch (type) {
    default:
        break;
    case LampEffectType::FLASH:
        updateFlash(lampIndex);
        break;
    case LampEffectType::GLOW:
        updateGlow(lampIndex);
        break;
    }
}
