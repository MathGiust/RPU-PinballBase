/*********************************************************************


Skill Shot
Version : 1.00


*********************************************************************/

#include "GameSpecific/GameModes/GameModes.h"

void SkillShot::onStart() {
}
void SkillShot::update() {
}
void SkillShot::onEnd() {
}
void SkillShot::handleSwitchHit(const uint8_t switchHit) {
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

void SkillShot::updateLamps() {
    if(!lampsNeedUpdate) return;
    lampsNeedUpdate = false;
}
