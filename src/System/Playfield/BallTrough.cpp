#include "BallTrough.h"

BallTrough::BallTrough(
        const uint8_t _ballTroughSize, const uint8_t* _ballTroughSwitches,
        const uint8_t _ballLockSize, const uint8_t* _ballLockSwitches,
        const uint8_t _outholeSwitch
) : ballTroughSize(_ballTroughSize), ballTroughSwitches(_ballTroughSwitches), ballTroughDebounce(nullptr), ballsInTroughStatus(nullptr),
    ballLockSize(_ballLockSize), ballLockSwitches(_ballLockSwitches), ballLockDebounce(nullptr), ballsInLockStatus(nullptr),
    outholeSwitch(_outholeSwitch), outholeDebounce(_outholeSwitch), outholeStatus(false) {
}
BallTrough::~BallTrough() {
}

void BallTrough::update() {
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        ballTroughDebounce[i].update();
        if (ballTroughDebounce[i].getDebounceState() == DEBOUNCE_HIGH) {
            this->ballsInTroughStatus[i] = true;
        } else if (ballTroughDebounce[i].getDebounceState() == DEBOUNCE_LOW) {
            this->ballsInTroughStatus[i] = false;
        }
    }
    for (uint8_t i = 0; i < this->ballLockSize; i++) {
        this->ballLockDebounce[i].update();
        if (ballLockDebounce[i].getDebounceState() == DEBOUNCE_HIGH) {
            this->ballsInLockStatus[i] = true;
        } else if (ballLockDebounce[i].getDebounceState() == DEBOUNCE_LOW) {
            this->ballsInLockStatus[i] = false;
        }
    }

    this->outholeDebounce.update();
    if (outholeDebounce.getDebounceState() == DEBOUNCE_HIGH) {
        this->outholeStatus = true;
    } else if (outholeDebounce.getDebounceState() == DEBOUNCE_LOW) {
        this->outholeStatus = false;
    }
}
void BallTrough::startDebounce() {
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        this->ballTroughDebounce[i].startDebounce();
    }
    for (uint8_t i = 0; i < this->ballLockSize; i++) {
        this->ballLockDebounce[i].startDebounce();
    }
    outholeDebounce.startDebounce();
}
void BallTrough::resetDebounce() {
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        ballTroughDebounce[i].resetStatus();
    }
    for (uint8_t i = 0; i < this->ballLockSize; i++) {
        ballLockDebounce[i].resetStatus();
    }
    outholeDebounce.resetStatus();
}

bool BallTrough::isDebouncePending() const {
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        if (this->ballTroughDebounce[i].getDebounceState() != DEBOUNCE_HIGH && this->ballTroughDebounce[i].getDebounceState() != DEBOUNCE_LOW) return false;
    }
    for (uint8_t i = 0; i < this->ballLockSize; i++) {
        if (this->ballLockDebounce[i].getDebounceState() != DEBOUNCE_HIGH && this->ballLockDebounce[i].getDebounceState() != DEBOUNCE_LOW) return false;
    }
    if (outholeDebounce.getDebounceState() != DEBOUNCE_HIGH && outholeDebounce.getDebounceState() != DEBOUNCE_LOW) return false;
    return true;
}
bool BallTrough::isDebounceIdle() const {
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        if (this->ballTroughDebounce[i].getDebounceState() != DEBOUNCE_IDLE) return false;
    }
    for (uint8_t i = 0; i < this->ballLockSize; i++) {
        if (this->ballLockDebounce[i].getDebounceState() != DEBOUNCE_IDLE) return false;
    }
    if (outholeDebounce.getDebounceState() != DEBOUNCE_IDLE) return false;
    return true;
}
bool BallTrough::getTroughStatus(const uint8_t index) const {
    if (index >= this->ballTroughSize) return false;
    return this->ballsInTroughStatus[index];
}
bool BallTrough::getBallLockStatus(const uint8_t index) const {
    if (index >= this->ballLockSize) return false;
    return this->ballsInLockStatus[index];
}
bool BallTrough::getOutholeStatus() const {
    return this->outholeStatus;
}

uint8_t BallTrough::getCurrentNumbersOfBallsInThrough() const {
    uint8_t returnValue = 0;
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        if (RPU_ReadSingleSwitchState(this->ballTroughSwitches[i])) returnValue++;
    }
    return returnValue;
}
uint8_t BallTrough::getCurrentNumbersOfBallsLocked() const {
    if (this->ballLockSize == 0) return 0;
    uint8_t returnValue = 0;
    for (uint8_t i = 0; i < this->ballTroughSize; i++) {
        if (RPU_ReadSingleSwitchState(this->ballTroughSwitches[i])) returnValue++;
    }
    return returnValue;
}
