#ifndef GS_PLAYER_STATE_H
#define GS_PLAYER_STATE_H

#include "System/Lamps/LampsHelper.h"
#include "System/MachineState.h"
#include "System/Scoring.h"
#include "System/Utilities.h"

struct Grid {
    uint8_t grid[3][3]{{0}};
    uint8_t lockedLines = 0;

    bool    isCellLocked(uint8_t x, uint8_t y) const;
    bool    isComplete(uint8_t a, uint8_t b, uint8_t c);
    uint8_t checkCompletedLines();
    uint8_t handleGridHit(uint8_t x, uint8_t y, uint8_t type);
};

class PlayerState {
    uint8_t bumperCount{0};
    uint8_t spinnerCount{0};

    uint8_t completedLineCount = 0;

    Grid grid;
    bool stars[4]{false};

public:
    score_t score{0};
    uint8_t bonusMultiplier{1};
    uint8_t bonus{0};

    void resetGrid();
    void reset();
    void initNewBall();

    void handleGridHit(uint8_t x, uint8_t y, bool eye);
    void showEyeGrid() const;
    void showPyramidGrid() const;
    void showCompletedLines() const {
        if (completedLineCount) LampsHelper::showOneLampFromCollection(LAMP_COLL_BONUS_LAMPS, completedLineCount - 1, 0);
    }
};

#endif // GS_PLAYER_STATE_H
