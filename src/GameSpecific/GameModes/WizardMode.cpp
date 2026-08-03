/*********************************************************************


Main Wizard mode
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void WizardMode::onStart() {
}
void WizardMode::update() {
}
void WizardMode::onEnd() {
}
void WizardMode::handleSwitchHit(const uint8_t switchHit) {
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

void WizardMode::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
