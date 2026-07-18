/*********************************************************************


    Attract State Source File
    Version : 1.00


*********************************************************************/

#include "GameSpecific/AttractState.h"

#include "GameSpecific/PinballMachineBase/Lamps.h"
#include "GameSpecific/PinballMachineBase/PinballBase.h"
#include "GameSpecific/RPU_Config.h"
#include "System/Display.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Log.h"
#include "System/REB.h"
#include "System/RPU.h"
#include "System/Sound/Sound.h"
#include "System/Sound/WaveTriggerHandler.h"
#include "System/Time.h"

#include <Arduino.h>

static Time::time_t currentStateStartTime = 0;

boolean featureShowRunning;
boolean updateScores;

byte currentFeatureCycle;

static unsigned long    currentFlashCycle;
static unsigned long    lastFlash;
static volatile score_t scores[4];

static void manageNewState(MachineState& machineState);

static void manageDisplays(MachineState& machineState);
static void manageLightShow(MachineState& machineState);
static void manageSoundShow(MachineState& machineState);

//
//  Public :
//

int Attract::run(const boolean currentStateChanged, MachineState& machineState) {
    if (currentStateChanged) manageNewState(machineState);

    manageLightShow(machineState);
    manageDisplays(machineState);
    // manageSoundShow(machineState);

    byte switchHit;
    while ((switchHit = RPU_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY) {
        switch (switchHit) {

        case (SW_CREDIT_BUTTON):
            return machineState.manageCreditButton(MACHINE_STATE_ATTRACT);
            break;
        case SW_COIN_1:
        case SW_COIN_2:
        case SW_COIN_3:
            MachineState::manageCoinDrop(switchHit);
            break;
        case SW_SELF_TEST_SWITCH:
            return MACHINE_STATE_TEST_LAMPS;
            break;
        default:
            break;
        }
    }

    if (RPU_ReadSingleSwitchState(SW_SAUCER) && Time::getCurrentTime() - Time::getLastSaucerSwitchHitTime() > 2000) {
        Time::updateLastSaucerSwitchHitTime();
        RPU_PushToTimedSolenoidStack(SOL_SAUCER, SOL_SAUCER_STRENGTH, Time::getCurrentTime() + 1000, true);
    }

    return MACHINE_STATE_ATTRACT;
}

//
//  Private :
//

void manageNewState(MachineState& machineState) {
#if (DEBUG_MESSAGES)
    Serial.write("Entering Attract State\n\r");
#endif

    currentStateStartTime = Time::getCurrentTime();

    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);

    LampsHelper::hideAllLamps();
    LampsHelper::setLampState(LAMP_GAME_OVER, true, 0, 0);

    machineState.exitGame();
    // MachineState::readEEPROM_Parameters();

    // Set last score to EEPROM, Check for High Score beaten
    score_t previousHighScore = machineState.getHighScore();
    uint8_t numberOfKnocks = 0;
    for (uint8_t i = 0; i < 4; i++) {
        scores[i] = machineState.getPlayerScore(i);
        RPU_WriteULToEEProm(RPU_PLAYER_1_SCORE_START_BYTE + i * 4, scores[i]);
        if (scores[i] > previousHighScore) {
            numberOfKnocks += 3;
            RPU_WriteULToEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE) + 1);
            RPU_WriteULToEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, scores[i]);
            machineState.setHighScore(scores[i]);
            previousHighScore = scores[i];
        }
    }

    for (uint8_t i = 0; i < numberOfKnocks; i++) {
        SoundHelper::pushToSoundQueue(0, Time::getCurrentTime() + (i + 1) * 200, 200, AUDIO_KNOCKER, 100);
    }

    // Display
    machineState.setNumberOfCreditsFromEEPROM();
    RPU_SetDisplayCredits(machineState.getCredits());

    if (!MachineState::getEEPROMDataPtr()->freeplay) DisplayHelper::setCreditDisplayValue(MachineState::getEEPROMDataPtr()->credits);
    else DisplayHelper::setCreditDisplayValue(0);

    updateScores = true;
    currentFlashCycle = 0;

    // Lamp animation
    LampsHelper::setLampState(LAMP_CREDIT_INDICATOR, MachineState::getEEPROMDataPtr()->freeplay || MachineState::getEEPROMDataPtr()->credits, 0, 200);

    // SOUND
    SoundHelper::Playlist::stopCurrent();
}

#define STEP_DURATION 50
#define NUMBER_OF_STEPS 24
void manageLightShow(MachineState& machineState) {
    static uint8_t      currentStep = 0;
    static Time::time_t lastStepTime = Time::getCurrentTime();
}

void manageDisplays(MachineState& machineState) {
    const unsigned long cycleSeed = (Time::getCurrentTime() - currentStateStartTime) / 5000; // 5 seconds
    const unsigned long seed = (Time::getCurrentTime() - currentStateStartTime) / 100;       // .10 seconds

    if (cycleSeed != currentFlashCycle) {
        currentFlashCycle = cycleSeed;
        updateScores = true;
    }

    if (seed != lastFlash) {
        lastFlash = seed;
        byte numberOfSteps = 16;
        byte currentStep = 0;

        if (currentFlashCycle % 2 == 1) {
            if (updateScores) {
                for (uint8_t i = 0; i < 4; i++) {
                    DisplayHelper::setScoreOverride(i, machineState.getHighScore(), true, 2);
                }
                LampsHelper::setLampState(LAMP_HIGH_SCORE, true, 0, 250);
                updateScores = false;
            }
        } else {
            if (updateScores) {
                for (uint8_t i = 0; i < 4; i++) {
                    DisplayHelper::setScoreOverride(i, machineState.getPlayerScore(i), true, 2);
                }
                LampsHelper::setLampState(LAMP_HIGH_SCORE, false, 0, 0);
                updateScores = false;
            }
        }
    }
}
