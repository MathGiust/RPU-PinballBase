/*********************************************************************


Game Mode 1 : Drop Rove
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void GameMode1::onStart() {
}
void GameMode1::update() {
}
void GameMode1::onEnd() {
}
void GameMode1::handleSwitchHit(const uint8_t switchHit) {
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

void GameMode1::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
