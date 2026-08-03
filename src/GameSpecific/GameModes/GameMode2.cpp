/*********************************************************************


Game Mode 2 : Drop Rove
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void GameMode2::onStart() {
}
void GameMode2::update() {
}
void GameMode2::onEnd() {
}
void GameMode2::handleSwitchHit(const uint8_t switchHit) {
    switch (switchHit) {
    default:
        break;
    }
}

/***************************************************************************************************/
//
// PRIVATE
//
/***************************************************************************************************/

void GameMode2::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
