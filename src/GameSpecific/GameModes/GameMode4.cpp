/*********************************************************************


Game Mode 4 : Drop Rove
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void GameMode4::onStart() {
}
void GameMode4::update() {
}
void GameMode4::onEnd() {
}
void GameMode4::handleSwitchHit(const uint8_t switchHit) {
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

void GameMode4::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
