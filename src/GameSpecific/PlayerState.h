#ifndef GS_PLAYER_STATE_H
#define GS_PLAYER_STATE_H

#include "System/Lamps/LampsHelper.h"
#include "System/MachineState.h"
#include "System/Scoring.h"
#include "System/Utilities.h"

struct PlayerState {
    score_t score{0};
    uint8_t bonusMultiplier{1};
    uint8_t bonus{0};

    void reset();
    void initNewBall();
};

#endif // GS_PLAYER_STATE_H
