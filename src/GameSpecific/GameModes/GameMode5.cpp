/*********************************************************************


Game Mode 5 :
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void GameMode5::onStart() {
}
void GameMode5::update() {
}
void GameMode5::onEnd() {
}
void GameMode5::handleSwitchHit(const uint8_t switchHit) {
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

void GameMode5::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
