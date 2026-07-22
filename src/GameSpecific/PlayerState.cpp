#include "PlayerState.h"

#include "GameSpecific/Gameplay.h"
#include "PinballMachineBase/EEPROM.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Utilities.h"

void PlayerState::reset() {
    score           = 0;
    bonusMultiplier = 1;
    bonus           = 1;

    resetGrid();
}
void PlayerState::initNewBall() {
    bonusMultiplier = 1;
    bonus           = 1;

    if (!holdGrid) resetGrid();
}

void PlayerState::resetGrid() {
    for (uint8_t x = 0; x < 3; ++x) {
        for (uint8_t y = 0; y < 3; ++y) {
            eyeGrid[x][y]     = false;
            pyramidGrid[x][y] = false;
        }
    }
}
void PlayerState::handleGridHit(const uint8_t x, const uint8_t y, const bool eye) {
    eyeGrid[x][y]     = eye;
    pyramidGrid[x][y] = !eye;
}
void PlayerState::showEyeGrid() const {
    static constexpr uint8_t eyeGridLamps[3][3] = {
            {LAMP_CENTER_EYE_1, LAMP_CENTER_EYE_2, LAMP_CENTER_EYE_3},
            {LAMP_CENTER_EYE_4, LAMP_CENTER_EYE_5, LAMP_CENTER_EYE_6},
            {LAMP_CENTER_EYE_7, LAMP_CENTER_EYE_8, LAMP_CENTER_EYE_9},
    };

    for (uint8_t y = 0; y < 3; y++) {
        for (uint8_t x = 0; x < 3; x++) {
            LampsHelper::setLampState(eyeGridLamps[y][x], eyeGrid[y][x], 0, 0);
        }
    }
}
void PlayerState::showPyramidGrid() const {
    static constexpr uint8_t pyramidGridLamps[3][3] = {
            {LAMP_CENTER_PYRAMID_1, LAMP_CENTER_PYRAMID_2, LAMP_CENTER_PYRAMID_3},
            {LAMP_CENTER_PYRAMID_4, LAMP_CENTER_PYRAMID_5, LAMP_CENTER_PYRAMID_6},
            {LAMP_CENTER_PYRAMID_7, LAMP_CENTER_PYRAMID_8, LAMP_CENTER_PYRAMID_9},
    };

    for (uint8_t y = 0; y < 3; y++) {
        for (uint8_t x = 0; x < 3; x++) {
            LampsHelper::setLampState(pyramidGridLamps[y][x], pyramidGrid[y][x], 0, 0);
        }
    }
}
