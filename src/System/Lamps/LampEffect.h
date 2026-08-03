#ifndef RPU_LAMP_EFFECT_H
#define RPU_LAMP_EFFECT_H
#include "System/Time.h"

#include <stdint.h>

enum class LampEffectType : uint8_t {
    NONE = 0,
    FLASH,
    GLOW
};

class LampEffect {

    void decreaseNumberOfCycleRemaining();
    void computeNextPeriodDuration();
    void checkIfCycleCompleted(uint8_t numberOfSteps);

    void computeNextFlashStepTime();
    void updateFlash(uint8_t lampIndex);

    void computeNextGlowStepTime();
    void updateGlow(uint8_t lampIndex);

public:
    LampEffectType type;

    bool         active : 1;
    bool         flashPolarity : 1;
    uint8_t      cycleRemaining = 0;
    uint8_t      cycleStep = 0;
    uint16_t     cyclePeriod = 0;
    int16_t      cycleAcceleration = 0;
    Time::time_t nextCycleStepTime = 0;
    Time::time_t nextCycleTime = 0;
    Time::time_t effectStartTime = 0;

    uint8_t flashDutyCycle = 0;

    LampEffect() : type(LampEffectType::NONE),
                   active(false),
                   flashPolarity(false) {}
    ~LampEffect() = default;

    void setLampEffect(
            LampEffectType effect,
            Time::time_t   startTime,
            uint8_t        numberOfCycle,
            uint16_t       period,
            int16_t        acceleration
    );
    void setFlash(
            Time::time_t startTime,
            uint8_t      numberOfCycle,
            uint16_t     period,
            bool         polarity,
            uint8_t      dutyCycle,
            int16_t      acceleration
    );
    void setGlow(
            Time::time_t startTime,
            uint8_t      numberOfCycle,
            uint16_t     period,
            int16_t      acceleration
    );
    void stop();

    void update(uint8_t lampIndex);
};

#endif // RPU_LAMP_EFFECT_H
