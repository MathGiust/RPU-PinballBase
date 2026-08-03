#include "LampAnimation.h"

#include "AnimationData.h"
#include "System/Lamps/LampsHelper.h"

#include <avr/pgmspace.h>

extern LampAnimation* animationList[];

namespace {
uint8_t            currentAnimation     = 0xFF; // 0xFF = no animation running
uint8_t            currentFrame         = 0;
Time::time_t       lastFrameTime        = 0;
Time::time_t       currentFrameDuration = 100;
AnimationDirection currentDirection     = AnimationDirection::FORWARD;
uint8_t            runsRemaining        = 0;
bool               infiniteRuns         = false;
bool               pingPongForward      = true;

void applyFrame(const LampAnimation* anim, const uint8_t frameIndex) {
    for (uint8_t byteIndex = 0; byteIndex < RPU_NUM_LAMP_BANKS; byteIndex++) {
        const uint8_t frameByte = pgm_read_byte(&anim->frames[frameIndex][byteIndex]);
        for (uint8_t bit = 0; bit < 8; bit++) {
            const uint8_t lampIndex = byteIndex * 8 + bit;
            if (lampIndex >= RPU_MAX_LAMPS) break;
            LampsHelper::setLampState(lampIndex, (frameByte >> bit) & 0x01, 0, 0);
        }
    }
}

// Returns true if a full run just completed (frame returned to its starting edge)
bool advanceFrame(const LampAnimation* anim) {
    switch (currentDirection) {
    default:
    case AnimationDirection::FORWARD:
        currentFrame++;
        if (currentFrame >= anim->numFrames) {
            currentFrame = 0;
            return true;
        }
        return false;

    case AnimationDirection::BACKWARD:
        if (currentFrame == 0) {
            currentFrame = anim->numFrames - 1;
            return true;
        }
        currentFrame--;
        return false;

    case AnimationDirection::PING_PONG:
        if (pingPongForward) {
            if (currentFrame + 1 >= anim->numFrames) {
                pingPongForward = false;
                if (anim->numFrames > 1) currentFrame--;
            } else {
                currentFrame++;
            }
        } else {
            if (currentFrame == 0) {
                pingPongForward = true;
                return true; // back at start = one full run
            }
            currentFrame--;
        }
        return false;
    }
}
} // namespace

void Animations::startAnimation(
        const uint8_t            animationNumber,
        const AnimationDirection direction,
        const uint8_t            numberOfRuns,
        const Time::time_t       frameDuration
) {
    if (animationNumber >= AnimationIDs::NUM_ANIMATIONS) return;

    currentAnimation     = animationNumber;
    currentDirection     = direction;
    infiniteRuns         = (numberOfRuns == 0);
    runsRemaining        = numberOfRuns;
    currentFrameDuration = frameDuration;
    pingPongForward      = true;
    currentFrame         = (direction == AnimationDirection::BACKWARD) ? animationList[currentAnimation]->numFrames - 1 : 0;
    lastFrameTime        = Time::getCurrentTime();

    applyFrame(animationList[currentAnimation], currentFrame);
}
void Animations::stopAnimation() {
    currentAnimation = 0xFF;
}

void Animations::update() {
    if (currentAnimation == 0xFF) return;
    if (Time::getCurrentTime() - lastFrameTime < currentFrameDuration) return;

    lastFrameTime = Time::getCurrentTime();

    const LampAnimation* anim         = animationList[currentAnimation];
    const bool           runCompleted = advanceFrame(anim);

    applyFrame(anim, currentFrame);

    if (runCompleted && !infiniteRuns) {
        runsRemaining--;
        if (runsRemaining == 0) stopAnimation();
    }
}
