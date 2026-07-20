#include "GameSpecific/AttractState.h"

#include "GameSpecific/PinballMachineBase/Constants.h"
#include "GameSpecific/PinballMachineBase/Lamps.h"
#include "PinballMachineBase/Solenoids.h"
#include "PinballMachineBase/Switches.h"
#include "System/Display.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Log.h"
#include "System/OperatorMenu/Adjustments.h"
#include "System/OperatorMenu/Audits.h"
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

static void initAttractState(MachineState& machineState);

static void updateDisplays(MachineState& machineState);
static void updateLightShow(MachineState& machineState);
static void updateSoundShow(MachineState& machineState);

//
//  Public :
//

void Attract::run(const boolean currentStateChanged, MachineState& machineState) {
    if (currentStateChanged) initAttractState(machineState);

    updateLightShow(machineState);
    updateDisplays(machineState);
    updateSoundShow(machineState);

    if (RPU_ReadSingleSwitchState(SW_SAUCER) && Time::getCurrentTime() - Time::getLastSaucerSwitchHitTime() > 2000) {
        Time::updateLastSaucerSwitchHitTime();
        RPU_PushToTimedSolenoidStack(SOL_SAUCER, SOL_SAUCER_STRENGTH, Time::getCurrentTime() + 1000, true);
    }
}
void Attract::handleSwitchHit(const uint8_t switchHit) {
    switch (switchHit) {
    default:
        break;
    }
}

//
//  Private :
//

void initAttractState(MachineState& machineState) {
    currentStateStartTime = Time::getCurrentTime();

    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);

    LampsHelper::hideAllLamps();
    LampsHelper::setLampState(LAMP_GAME_OVER, true, 0, 0);

    DisplayHelper::stopAllDisplayOverride();

    // Set last score to EEPROM, Check for High Score beaten
    score_t previousHighScore = machineState.getHighScore();
    uint8_t numberOfKnocks    = 0;
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

    if (!MachineState::EEPROMData::freeplay) DisplayHelper::setCreditDisplayValue(MachineState::EEPROMData::credits);
    else DisplayHelper::setCreditDisplayValue(0);

    updateScores      = true;
    currentFlashCycle = 0;

    // Lamp animation
    LampsHelper::setLampState(LAMP_CREDIT_INDICATOR, MachineState::EEPROMData::freeplay || MachineState::EEPROMData::credits, 0, 200);

    // SOUND
    SoundHelper::Playlist::stopCurrent();
}

#define STEP_DURATION 50
#define NUMBER_OF_STEPS 24
void updateLightShow(MachineState& machineState) {
}
void updateDisplays(MachineState& machineState) {
    const unsigned long cycleSeed = (Time::getCurrentTime() - currentStateStartTime) / 5000; // 5 seconds
    const unsigned long seed      = (Time::getCurrentTime() - currentStateStartTime) / 100;  // .10 seconds

    if (cycleSeed != currentFlashCycle) {
        currentFlashCycle = cycleSeed;
        updateScores      = true;
    }

    if (seed != lastFlash) {
        lastFlash = seed;
        // byte numberOfSteps = 16;
        // byte currentStep   = 0;

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
void updateSoundShow(MachineState& machineState) {
}
