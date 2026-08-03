#include "MachineState.h"

#include "Display.h"
#include "Lamps/LampsHelper.h"
#include "Log.h"
#include "OperatorMenu/Audits.h"
#include "RPU.h"
#include "Scoring.h"
#include "Time.h"

// Static members
uint8_t MachineState::mostRecentSwitchHit   = 0xFF;
bool    MachineState::samePlayerShootsAgain = false;

bool MachineState::EEPROMData::freeplay = false;

uint8_t MachineState::EEPROMData::credits              = 0;
uint8_t MachineState::EEPROMData::ballSaveDuration     = 0;
uint8_t MachineState::EEPROMData::numberOfTiltWarnings = 0;

uint8_t MachineState::EEPROMData::noveltyMode    = NOVELTY_POINTS_AND_EB;
score_t MachineState::EEPROMData::extraBallScore = 25000;
score_t MachineState::EEPROMData::specialScore   = 50000;

uint8_t MachineState::EEPROMData::knockerMode = KNOCKER_SOUND_AND_SOL;
uint8_t MachineState::EEPROMData::overallGain = 0;
uint8_t MachineState::EEPROMData::SFX_Gain    = 0;
uint8_t MachineState::EEPROMData::VOICE_Gain  = 0;
uint8_t MachineState::EEPROMData::MUSIC_Gain  = 0;

score_t MachineState::EEPROMData::highScore       = 0;
score_t MachineState::EEPROMData::replayScores[3] = {0, 0, 0};

MachineState::MachineState(
        const uint8_t _maxNumberOfPlayers, score_t* const* _scores,
        const uint8_t coinDrop1, const uint8_t coinDrop2, const uint8_t coinDrop3,
        const uint8_t _numberOfDropTargetBanks, DropTargetBank* const* _dropTargetBanks,
        const uint8_t _numberOfStandupTargetBanks, StandupTargetBank* const* _standupTargetBanks,
        const uint8_t _numberOfDebouncedSwitches, DebouncedSwitch* const* _debouncedSwitches,
        const uint8_t _outholeSwitch
)
    : maxNumberOfPlayers(_maxNumberOfPlayers), scores(_scores),
      outholeSwitch(_outholeSwitch),
      coinDropSwitches{coinDrop1, coinDrop2, coinDrop3},

      tilted(false), currentScoreFlashing(false),
      currentBallInPlay(0xFF), numberOfTiltWarningRemaining(0), numberOfPlayers(0),
      currentPlayerNumber(0xFF), playfieldValidated(false), numberOfBallsInPlay(0),
      gameStartTime(0), ballStartTime(0), ballSaveStartTime(0), ballInSaucer(false),

      numberOfDropTargetBanks(_numberOfDropTargetBanks),
      DTB(_dropTargetBanks),

      numberOfStandupTargetBanks(_numberOfStandupTargetBanks),
      STB(_standupTargetBanks),

      numberOfDebouncedSwitches(_numberOfDebouncedSwitches),
      DEB(_debouncedSwitches) {}

// Game State Functions
void MachineState::resetGame() {
    currentBallInPlay   = 0;
    numberOfPlayers     = 0;
    currentPlayerNumber = 0;
}
void MachineState::initNewGame() {
    currentBallInPlay   = 1;
    currentPlayerNumber = 0;
    addPlayer();
}
void MachineState::initNewBall() {
    ballStartTime                = Time::getCurrentTime();
    ballSaveStartTime            = 0;
    tilted                       = false;
    numberOfTiltWarningRemaining = EEPROMData::numberOfTiltWarnings;
    setSamePlayerShootsAgain(false);
    setPlayfieldValidated(false);
    RPU_SetDisableFlippers(false);
    RPU_EnableSolenoidStack();

    Scoring::emptyAllStacks();
    Scoring::setScoringState(true);

    DisplayHelper::stopAllDisplayOverride();
    DisplayHelper::setBallInPlayDisplayValue(currentBallInPlay);
    DisplayHelper::blankAllDisplays();

    LampsHelper::stopAllLampEffects();
    LampsHelper::stopAllLampAnimations();

    setMostRecentSwitchHit(0xFF);
}

// System switches
void MachineState::manageCoinDrop(const uint8_t switchHit) const {
    if (EEPROMData::freeplay) {
        DisplayHelper::setCreditDisplayValue(0);
        return;
    }

    if (EEPROMData::credits < 99) EEPROMData::credits += 1;
    else EEPROMData::credits = 99;
    DisplayHelper::setCreditDisplayValue(EEPROMData::credits);

    RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, EEPROMData::credits);

    unsigned long nbOfChute;
    if (switchHit == coinDropSwitches[0]) {
        nbOfChute = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE);
        RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE, nbOfChute + 1);
    } else if (switchHit == coinDropSwitches[1]) {
        nbOfChute = RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE);
        RPU_WriteULToEEProm(RPU_CHUTE_2_COINS_START_BYTE, nbOfChute + 1);
    } else if (switchHit == coinDropSwitches[2]) {
        nbOfChute = RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE);
        RPU_WriteULToEEProm(RPU_CHUTE_3_COINS_START_BYTE, nbOfChute + 1);
    }
}
void MachineState::manageTiltWarning() {
    if (Time::getCurrentTime() - Time::getLastTiltWarningTime() > TILT_WARNING_DEBOUNCE_TIME) {
        Time::updateLastTiltWarningTime();

        if (numberOfTiltWarningRemaining) {
            numberOfTiltWarningRemaining--;
        } else {
            tilted = true;
            // currentPlayer->setBonusValue(0);
            RPU_DisableSolenoidStack();
            RPU_SetDisableFlippers(true);
        }
    }
}

// Game handling
bool MachineState::isEnoughCredit() {
    return EEPROMData::credits || EEPROMData::freeplay;
}
void MachineState::addPlayer() {
    if (numberOfPlayers >= maxNumberOfPlayers) return;

    numberOfPlayers++;
    if (!EEPROMData::freeplay) {
        EEPROMData::credits -= 1;
        DisplayHelper::setCreditDisplayValue(EEPROMData::credits);
        RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, EEPROMData::credits);
    }

    RPU_WriteByteToEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE) + 1);

    Log::printVariable(Log::LOG_MACHINE_STATE, "Number of Players", getNumberOfPlayers());
}
bool MachineState::increaseCurrentPlayerNumber() {
    if (currentPlayerNumber == numberOfPlayers - 1) {
        if (currentBallInPlay >= 3) return true;
        currentBallInPlay++;
        currentPlayerNumber = 0;
    } else currentPlayerNumber++;
    return false;
}

// Award EB and SPECIAL
void MachineState::awardExtraBall() {
    if (EEPROMData::noveltyMode == NOVELTY_POINTS_AND_EB || EEPROMData::noveltyMode == NOVELTY_POINTS_ONLY) Scoring::addScoreToStacks(EEPROMData::extraBallScore);
    if (EEPROMData::noveltyMode == NOVELTY_POINTS_AND_EB || EEPROMData::noveltyMode == NOVELTY_EB_ONLY) samePlayerShootsAgain = true;
}
void MachineState::awardSpecial() {
    if (EEPROMData::noveltyMode == NOVELTY_POINTS_AND_EB || EEPROMData::noveltyMode == NOVELTY_POINTS_ONLY) Scoring::addScoreToStacks(EEPROMData::specialScore);
    if (EEPROMData::noveltyMode == NOVELTY_POINTS_AND_EB || EEPROMData::noveltyMode == NOVELTY_EB_ONLY) {}
}
