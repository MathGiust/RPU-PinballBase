#ifndef LAMP_ANIMATIONS_H
#define LAMP_ANIMATIONS_H

#include "GameSpecific/RPU_Config.h"

#include <stdint.h>

enum class AnimationDirection : uint8_t {
    FORWARD = 0,
    BACKWARD,
    PING_PONG
};

struct LampAnimation {
    const uint8_t (*const frames)[RPU_NUM_LAMP_BANKS]; // PROGMEM data
    const uint8_t numFrames;

    constexpr LampAnimation(const uint8_t (*frameData)[RPU_NUM_LAMP_BANKS], const uint8_t frameCount)
        : frames(frameData), numFrames(frameCount) {}
};

namespace Animations {
void update();
void startAnimation(uint8_t animationNumber, AnimationDirection direction = AnimationDirection::FORWARD, uint8_t numberOfRuns = 0);
void stopAnimation();
} // namespace Animations

#endif // LAMP_ANIMATIONS_H
