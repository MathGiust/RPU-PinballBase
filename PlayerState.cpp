#include "PlayerState.h"

#include "System/Utilities.h"

// TODO : Reset player for new game
void PlayerState::resetPlayer() {
    score           = 0;
    bonusMultiplier = 1;
    bonus           = 0;

    Utilities::resetArray(centerGLOBE_Letters, 5);
    Utilities::resetArray(saucerGLOBE_Letters, 5);
}
// TODO : Init new ball
void PlayerState::initNewBall() {
    bonusMultiplier = 1;
    bonus           = 1;

    //
    // Game specific variables
    //

    centerSpinnerProgress
            = 0;
    centerLeftSpinnerLit  = true;
    centerRightSpinnerLit = true;
    leftSpinnerValue      = 0;
    Utilities::resetArray(saucerGLOBE_Letters, 5);
    saucerGLOBE_Letters[4] = true;
}

// Set and get
uint8_t PlayerState::getDunkShotProgress() const {
    return dunkShotProgress;
}
void PlayerState::setDunkShotProgress(const uint8_t dunk_shot_progress) {
    dunkShotProgress = dunk_shot_progress;
}
uint8_t PlayerState::getCenterSpinnerProgress() const {
    return centerSpinnerProgress;
}
void PlayerState::setCenterSpinnerProgress(const uint8_t center_spinner_progress) {
    centerSpinnerProgress = center_spinner_progress;
}
uint8_t PlayerState::getLeftSpinnerValue() const {
    return leftSpinnerValue;
}
void PlayerState::setLeftSpinnerValue(const uint8_t left_spinner_value) {
    leftSpinnerValue = left_spinner_value;
}
bool PlayerState::isCenterLeftSpinnerLit() const {
    return centerLeftSpinnerLit;
}
void PlayerState::setCenterLeftSpinnerLit(const bool center_left_spinner_lit) {
    centerLeftSpinnerLit = center_left_spinner_lit;
}
bool PlayerState::isCenterRightSpinnerLit() const {
    return centerRightSpinnerLit;
}
void PlayerState::setCenterRightSpinnerLit(const bool center_right_spinner_lit) {
    centerRightSpinnerLit = center_right_spinner_lit;
}
bool PlayerState::isGateOpen() const {
    return gateOpen;
}
void PlayerState::setGateOpen(const bool gate_open) {
    gateOpen = gate_open;
}
bool PlayerState::isLeftOutlaneLit() const {
    return LeftOutlaneLit;
}
void PlayerState::setLeftOutlaneLit(const bool left_outlane_lit) {
    LeftOutlaneLit = left_outlane_lit;
}

// Load Array
bool* PlayerState::getSaucerGLOBE_Array() {
    return saucerGLOBE_Letters;
}
bool* PlayerState::getCenterGLOBE_Array() {
    return centerGLOBE_Letters;
}
void PlayerState::setCenterGLOBE_Letter(uint8_t letterToSet, bool value) {
    if (letterToSet > 4) return;
    centerGLOBE_Letters[letterToSet] = value;
}
