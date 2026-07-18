#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "GameSpecific/Lamps.h"
#include "PinballMachineBase/Lamps.h"
#include "System/LampAnimations.h"

// Create a single step: array of lamps and a step structure
#define STEP(stepName, ...)                                                      \
    constexpr uint8_t                        stepName##_lamps[] = {__VA_ARGS__}; \
    constexpr AnimationHelper::AnimationStep stepName = {stepName##_lamps, sizeof(stepName##_lamps) / sizeof(uint8_t)};

// Create an animation: array of steps and animation structure
#define ANIMATION(animationName, statusRef, ...)                                        \
    constexpr AnimationHelper::AnimationStep animationName##_steps[] = {__VA_ARGS__};   \
    constexpr AnimationHelper::Animation     animationName = {                          \
            sizeof(animationName##_steps) / sizeof(AnimationHelper::AnimationStep), \
            animationName##_steps,                                                  \
            statusRef                                                               \
    };

/********************/
/*  Top to bottom   */
/********************/

AnimationHelper::AnimationStatus animationParameters[NUMBER_OF_LAMP_ANIMATIONS];
ANIMATION(centerBonus, &animationParameters[0], );
// End of top to bottom

constexpr AnimationHelper::Animation animations[NUMBER_OF_LAMP_ANIMATIONS] = {
        centerBonus
};

/*************************************************************************************************************************

Seed based animations

/*************************************************************************************************************************/

struct SeedAnimations {
    uint8_t      numberOfSteps;
    Time::time_t animationStartTime;
    Time::time_t currentSeed;

    explicit SeedAnimations(const uint8_t numberOfSteps)
        : numberOfSteps(numberOfSteps),
          animationStartTime(0),
          currentSeed(0) {};
    ~SeedAnimations();
};

inline void updateBonusSweeping() {
    static SeedAnimations bonusSweeping(12);

}

#endif // ANIMATIONS_H
