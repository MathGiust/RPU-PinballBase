/*********************************************************************


Game Mode 3 : Drop Rove
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void GameMode3::onStart() {
}
void GameMode3::update() {
}
void GameMode3::onEnd() {
}
void GameMode3::handleSwitchHit(const uint8_t switchHit) {
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

void GameMode3::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
