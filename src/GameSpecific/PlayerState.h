#ifndef GS_PLAYER_STATE_H
#define GS_PLAYER_STATE_H

#include "System/Lamps/LampsHelper.h"
#include "System/MachineState.h"
#include "System/Scoring.h"
#include "System/Utilities.h"

class PlayerState {
    uint8_t bumperCount{0};
    uint8_t spinnerCount{0};

    bool eyeGrid[3][3]{{false}};
    bool pyramidGrid[3][3]{{false}};
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
};

#endif // GS_PLAYER_STATE_H
