/*********************************************************************


    Bonus Count Source File
    Version : 1.00


*********************************************************************/

#include "GameSpecific/BonusCountDown.h"

#include "System/Scoring.h"

#define PHASE_INIT 0
#define PHASE_COUNT_DOLLY 1
#define PHASE_COUNT_PARTON 2
#define PHASE_COUNT_SUPER 3

#define BONUS_SCORING_DELAY 100
#define SUPER_BONUS_DELAY 150

static void manageNewState(MachineState& machineState);
static void updateLamps(MachineState& machineState);

void BonusCountDown::run(const boolean curStateChanged, MachineState& machineState) {
    if (curStateChanged) manageNewState(machineState);

    Scoring::updateScoring();
    updateLamps(machineState);
}

//
//  Static
//

void manageNewState(MachineState& machineState) {
}

void updateLamps(MachineState& machineState) {
}
