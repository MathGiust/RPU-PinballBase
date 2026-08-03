#ifndef LAMP_ANIMATIONS_H
#define LAMP_ANIMATIONS_H

#include "GameSpecific/RPU_Config.h"
#include "System/Time.h"

#include <stdint.h>

enum class AnimationDirection : uint8_t {
    FORWARD = 0,
    BACKWARD,
    PING_PONG
};

struct LampAnimation {
    // Constants
    const uint8_t (*const frames)[RPU_NUM_LAMP_BANKS]; // PROGMEM data
    const uint8_t numFrames;

    // Parameters
    bool               running       = false;
    AnimationDirection direction     = AnimationDirection::FORWARD;
    Time::time_t       frameDuration = 100;

    // Animation run
    uint8_t      currentFrame    = 0;
    uint8_t      numberOfRuns    = 0;
    Time::time_t lastFrameTime   = 0;
    bool         pingPongForward = false;

    LampAnimation(const uint8_t (*frameData)[RPU_NUM_LAMP_BANKS], const uint8_t frameCount)
        : frames(frameData),
          numFrames(frameCount) {}

    void update();
    void start(AnimationDirection _direction, uint8_t _numberOfRuns, Time::time_t _frameDuration);
};

#endif // LAMP_ANIMATIONS_H
