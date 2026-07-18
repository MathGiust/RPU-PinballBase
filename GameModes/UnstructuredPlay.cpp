/*********************************************************************


Unstructured play
Version : 1.00


*********************************************************************/

#include "GameSpecific//GameModes/GameModes.h"
#include "GameSpecific/Lamps.h"
#include "GameSpecific/PinballMachineBase/Playfield.h"
#include "GameSpecific/PinballMachineBase/Solenoids.h"
#include "GameSpecific/PinballMachineBase/Sound.h"
#include "GameSpecific/PinballMachineBase/Switches.h"
#include "System/Scoring.h"
#include "System/Sound/Sound.h"
#include "System/Utilities.h"

// Game mode variables :
void UnstructuredPlay::onStart() {
    Serial.println("-- Entering Unstructured play --");
    currentPlayer = &players[machineState->getCurrentPlayerNumber()];

    // Music and sound :
    if (machineState->getPlayfieldValidated()) {
    }
}
void UnstructuredPlay::update() {
    this->updateLamps();
}
void UnstructuredPlay::onEnd() {
}
void UnstructuredPlay::handleSwitchHit(const uint8_t switchHit) {
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

void UnstructuredPlay::updateLamps() {
}
