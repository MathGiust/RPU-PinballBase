#include "PlayerState.h"

#include "GameSpecific/Gameplay.h"
#include "GameSpecific/Lamps.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Utilities.h"

// TODO : Reset player for new game
void PlayerState::reset() {
    score           = 0;
    bonusMultiplier = 1;
    bonus           = 1;
}
// TODO : Init new ball
void PlayerState::initNewBall() {
    bonusMultiplier = 1;
    bonus           = 1;
}
