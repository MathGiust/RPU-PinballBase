#include "Animations.h"

void Animations::startLampAnimation(
        const AnimationIDs       animationNumber,
        const AnimationDirection direction,
        const uint8_t            numberOfRuns,
        const Time::time_t       frameDuration
) {
    animationList[animationNumber]->start(direction, numberOfRuns, frameDuration);
}
void Animations::stopLampAnimation(AnimationIDs animationNumber) {
    animationList[animationNumber]->running = false;
}
void Animations::update() {
    for (auto& animation : animationList) {
        animation->update();
    }
}
