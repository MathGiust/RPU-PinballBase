/*********************************************************************


Game Mode 6 : Drop Rove
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void GameMode6::onStart() {
}
void GameMode6::update() {
}
void GameMode6::onEnd() {
}
void GameMode6::handleSwitchHit(const uint8_t switchHit) {
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

void GameMode6::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
