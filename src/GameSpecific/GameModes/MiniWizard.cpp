/*********************************************************************


Mini Wizard mode
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"
// Static prototypes :

/***************************************************************************************************/
//
// PUBLIC
//
/***************************************************************************************************/
void MiniWizard::onStart() {
}
void MiniWizard::update() {
}
void MiniWizard::onEnd() {
}
void MiniWizard::handleSwitchHit(const uint8_t switchHit) {
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

void MiniWizard::updateLamps() {
    if (!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
