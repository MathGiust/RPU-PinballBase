#include "PlayerState.h"

#include "GameSpecific/Gameplay.h"
#include "PinballMachineBase/Adjustments.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Utilities.h"

void PlayerState::reset() {
    score           = 0;
    bonusMultiplier = 1;
    bonus           = 1;

    completedLineCount = 0;
    resetGrid();
}
void PlayerState::initNewBall() {
    bonusMultiplier = 1;
    bonus           = 1;

    if (!GS_EEPROM::holdGrid) resetGrid();
}

void PlayerState::resetGrid() {
    for (auto& x : grid.grid) {
        for (unsigned char& y : x) {
            y = 0;
        }
    }
    grid.lockedLines = 0;
}

enum {
    ROW0,
    ROW1,
    ROW2,
    COL0,
    COL1,
    COL2,
    DIAG_MAIN,
    DIAG_ANTI
};

bool Grid::isComplete(const uint8_t a, const uint8_t b, const uint8_t c) {
    return (a != 0) && (a == b) && (b == c);
}
bool Grid::isCellLocked(const uint8_t x, const uint8_t y) const {
    if ((lockedLines & (1 << x))) return true;          // Row
    if ((lockedLines & (1 << (COL0 + y)))) return true; // Column

    if (x == y && (lockedLines & (1 << DIAG_MAIN)))
        return true;

    if (x + y == 2 && (lockedLines & (1 << DIAG_ANTI)))
        return true;

    return false;
}
uint8_t Grid::handleGridHit(const uint8_t x, const uint8_t y, const uint8_t type) {
    if (type > 2)
        return false;

    if (isCellLocked(x, y))
        return false;

    grid[x][y] = type;

    return checkCompletedLines();
}
uint8_t Grid::checkCompletedLines() {
    uint8_t completedLines = 0;
    // Rows
    for (uint8_t r = 0; r < 3; r++) {
        if (!(lockedLines & (1 << r)) && isComplete(grid[r][0], grid[r][1], grid[r][2])) {
            lockedLines |= (1 << r);
            completedLines++;
        }
    }

    // Columns
    for (uint8_t c = 0; c < 3; c++) {
        const uint8_t bit = COL0 + c;

        if (!(lockedLines & (1 << bit)) && isComplete(grid[0][c], grid[1][c], grid[2][c])) {
            lockedLines |= (1 << bit);
            completedLines++;
        }
    }

    // Main diagonal
    if (!(lockedLines & (1 << DIAG_MAIN)) && isComplete(grid[0][0], grid[1][1], grid[2][2])) {
        lockedLines |= (1 << DIAG_MAIN);
        completedLines++;
    }

    // Anti diagonal
    if (!(lockedLines & (1 << DIAG_ANTI)) && isComplete(grid[0][2], grid[1][1], grid[2][0])) {
        lockedLines |= (1 << DIAG_ANTI);
        completedLines++;
    }
    return completedLines;
}

void PlayerState::handleGridHit(const uint8_t x, const uint8_t y, const bool eye) {
    completedLineCount += grid.handleGridHit(x, y, eye ? 1 : 2);
    if (completedLineCount >= 8) completedLineCount = 8;
}
void PlayerState::showPyramidGrid() const {
    static constexpr uint8_t pyramidGridLamps[3][3] = {
            {LAMP_CENTER_PYRAMID_1, LAMP_CENTER_PYRAMID_2, LAMP_CENTER_PYRAMID_3},
            {LAMP_CENTER_PYRAMID_4, LAMP_CENTER_PYRAMID_5, LAMP_CENTER_PYRAMID_6},
            {LAMP_CENTER_PYRAMID_7, LAMP_CENTER_PYRAMID_8, LAMP_CENTER_PYRAMID_9},
    };

    for (uint8_t y = 0; y < 3; y++) {
        for (uint8_t x = 0; x < 3; x++) {
            LampsHelper::setLampState(pyramidGridLamps[y][x], grid.grid[y][x] == 2, 0, 0);
        }
    }
}
void PlayerState::showEyeGrid() const {
    static constexpr uint8_t eyeGridLamps[3][3] = {
            {LAMP_CENTER_EYE_1, LAMP_CENTER_EYE_2, LAMP_CENTER_EYE_3},
            {LAMP_CENTER_EYE_4, LAMP_CENTER_EYE_5, LAMP_CENTER_EYE_6},
            {LAMP_CENTER_EYE_7, LAMP_CENTER_EYE_8, LAMP_CENTER_EYE_9},
    };

    for (uint8_t y = 0; y < 3; y++) {
        for (uint8_t x = 0; x < 3; x++) {
            LampsHelper::setLampState(eyeGridLamps[y][x], grid.grid[y][x] == 1, 0, 0);
        }
    }
}
