#include "LampAnimation.h"

#include "System/Lamps/LampsHelper.h"

#include <avr/pgmspace.h>

void applyFrame(LampAnimation* anim, const uint8_t frameIndex) {
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
bool advanceFrame(LampAnimation* anim) {
    switch (anim->direction) {
    default:
    case AnimationDirection::FORWARD:
        anim->currentFrame++;
        if (anim->currentFrame >= anim->numFrames) {
            anim->currentFrame = 0;
            return true;
        }
        return false;

    case AnimationDirection::BACKWARD:
        if (anim->currentFrame == 0) {
            anim->currentFrame = anim->numFrames - 1;
            return true;
        }
        anim->currentFrame--;
        return false;

    case AnimationDirection::PING_PONG:
        if (anim->pingPongForward) {
            if (anim->currentFrame + 1 >= anim->numFrames) {
                anim->pingPongForward = false;
                if (anim->numFrames > 1) anim->currentFrame--;
            } else {
                anim->currentFrame++;
            }
        } else {
            if (anim->currentFrame == 0) {
                anim->pingPongForward = true;
                return true; // back at start = one full run
            }
            anim->currentFrame--;
        }
        return false;
    }
}

void LampAnimation::update() {
    if (!running) return;
    if (Time::getCurrentTime() - lastFrameTime < frameDuration) return;

    lastFrameTime = Time::getCurrentTime();

    const bool runCompleted = advanceFrame(this);
    applyFrame(this, currentFrame);

    if (runCompleted && numberOfRuns != 0xFF) {
        numberOfRuns--;
        running = numberOfRuns;
    }
}
void LampAnimation::start(const AnimationDirection _direction, const uint8_t _numberOfRuns, const Time::time_t _frameDuration) {
    direction       = _direction;
    numberOfRuns    = _numberOfRuns;
    frameDuration   = _frameDuration;
    pingPongForward = true;
    currentFrame    = (direction == AnimationDirection::BACKWARD) ? numFrames - 1 : 0;
    lastFrameTime   = Time::getCurrentTime();

    applyFrame(this, currentFrame);
}
