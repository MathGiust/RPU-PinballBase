/*********************************************************************


    Bonus Count Source File
    Version : 1.00


*********************************************************************/

#include "GameSpecific/BonusCountDown.h"

#include "Lamps.h"
#include "PinballMachineBase/PinballBase.h"
#include "PlayerState.h"
#include "System/Display.h"
#include "System/Scoring.h"

static uint8_t  countDownPhase = 0;
static uint8_t  bonus          = 0;
static uint16_t superBonus     = 0;
static uint8_t  superBonusLamp = 0xFF;

static Time::time_t phaseTimeStart     = 0;
static Time::time_t lastBonusPointTime = 0;

uint8_t BonusCountDown::run(const bool curStateChanged, PlayerState* _currentPlayer) {
    PlayerState* currentPlayer = _currentPlayer;

    Time::time_t timeToWaitBeforeCount = 110;

    if (curStateChanged) {
        if (DEBUG_MESSAGES) Serial.write("Entering Bonus Countdown :\n\r");
        countDownPhase = 0;
        DisplayHelper::stopAllDisplayOverride();
    }

    switch (countDownPhase) {
    default:
        return MACHINE_STATE_BALL_OVER;
    }
    Scoring::updateScoring();
    return MACHINE_STATE_COUNTDOWN_BONUS;
}

//
//  Private :
//
