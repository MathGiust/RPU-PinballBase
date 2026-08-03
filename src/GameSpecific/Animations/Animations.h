#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "AnimationData.h"
#include "LampAnimation.h"

namespace Animations {
void update();
void startLampAnimation(
        AnimationIDs       animationNumber,
        AnimationDirection direction     = AnimationDirection::FORWARD,
        uint8_t            numberOfRuns  = 0,
        Time::time_t       frameDuration = 100
);
void stopLampAnimation(AnimationIDs animationNumber);
} // namespace Animations

#endif // ANIMATIONS_H
