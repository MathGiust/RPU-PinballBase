#include "Animations/LampAnimation.h"
#include "GameSpecific/AttractState.h"
#include "GameSpecific/BonusCountDown.h"
#include "GameSpecific/Gameplay.h"
#include "GameSpecific/PinballMachineBase/Adjustments.h"
#include "GameSpecific/PinballMachineBase/Playfield.h"
#include "GameSpecific/PinballMachineBase/Solenoids.h"
#include "GameSpecific/PlayerState.h"
#include "PinballMachineBase/Constants.h"
#include "PinballMachineBase/Sound.h"
#include "System/Display.h"
#include "System/GameState.h"
#include "System/Log.h"
#include "System/OperatorMenu/Adjustments.h"
#include "System/OperatorMenu/Audits.h"
#include "System/OperatorMenu/OperatorMenu.h"
#include "System/OperatorMenu/SelfTests.h"
#include "System/Sound/Sound.h"
#include "System/Sound/WaveTriggerHandler.h"

#include <Arduino.h>

#define EEPROM_RPU_OS_INIT_PROOF_UL 90
#define RPU_OS_INIT_PROOF 0x4D595331 // MYS1
#define GAME_MAJOR_VERSION 2026      // YEAR
#define GAME_MINOR_VERSION 720       // DAY-MONTH

PlayerState playerStates[4];
score_t*    scores[4]{
        &playerStates[0].score,
        &playerStates[1].score,
        &playerStates[2].score,
        &playerStates[3].score,
};

DropTargetBank dropBankLeft(
        3, DTB_LEFT_SWITCHES,
        1, DTB_LEFT_SOLS, SOL_DROP_RESET_STRENGTH,
        DTB_LEFT_STATUS
);
DropTargetBank dropBankMid(
        3, DTB_MID_SWITCHES,
        1, DTB_MID_SOLS, SOL_DROP_RESET_STRENGTH,
        DTB_MID_STATUS
);
DropTargetBank dropBankRight(
        3, DTB_RIGHT_SWITCHES,
        1, DTB_RIGHT_SOLS, SOL_DROP_RESET_STRENGTH,
        DTB_RIGHT_STATUS
);
DropTargetBank* dropTargetBanks[NUMBER_OF_DROP_TARGET_BANKS]{
        &dropBankLeft,
        &dropBankMid,
        &dropBankRight
};
StandupTargetBank* standupTargetBanks[NUMBER_OF_STANDUP_TARGET_BANKS]{};
DebouncedSwitch*   debouncedSwitches[NUMBER_OF_DEBOUNCED_SWITCHES]{};

MachineState machineState(
        4, scores,
        SW_COIN_1, SW_COIN_2, SW_COIN_3,
        NUMBER_OF_DROP_TARGET_BANKS, dropTargetBanks,
        NUMBER_OF_STANDUP_TARGET_BANKS, standupTargetBanks,
        NUMBER_OF_DEBOUNCED_SWITCHES, debouncedSwitches,
        SW_OUTHOLE
);

OperatorMenu operatorMenu(
        SW_COIN_1, SW_CREDIT_BUTTON, SW_SLAM,
        SOL_TEST_STRENGTH, NUMBER_OF_STD_TESTS,
        standardAuditTable, NUMBER_OF_STD_AUDITS,
        standardAdjustmentTable, NUMBER_OF_STD_ADJ,
        specificAdjustmentTable, NUMBER_OF_GS_ADJ
);

PlayfieldAndCabinetSwitch solenoidAssociatedSwitches[] = {
        {SW_RIGHT_SLING,   SOL_RIGHT_SLING,   SOL_SLING_STRENGTH },
        {SW_LEFT_SLING,    SOL_LEFT_SLING,    SOL_SLING_STRENGTH },
        {SW_LEFT_BUMPER,   SOL_LEFT_BUMPER,   SOL_BUMPER_STRENGTH},
        {SW_BOTTOM_BUMPER, SOL_BOTTOM_BUMPER, SOL_BUMPER_STRENGTH},
        {SW_RIGHT_BUMPER,  SOL_RIGHT_BUMPER,  SOL_BUMPER_STRENGTH}
};

static GameState initSystem();
static GameState runOperatorMenu();
static GameState runAttract();
static GameState initNewGame();
static GameState initNewBall();
static GameState runGameplay();
static GameState runBonusCountDown();

GameState initSystem() {
    static bool         checkSumOK = false;
    static Time::time_t initTime   = 0;

    if (GameState::hasChanged()) {
        Log::printMessage(Log::LOG_NONE, "Initializing System");
        initTime = Time::getCurrentTime();

        RPU_SetDisplay(0, GAME_MAJOR_VERSION, true, 0);
        RPU_SetDisplay(1, GAME_MINOR_VERSION, true, 0);
        RPU_SetDisplay(2, RPU_OS_MAJOR_VERSION, true, 0);
        RPU_SetDisplay(3, RPU_OS_MINOR_VERSION, true, 0);

        if (DEV_MODE) {
            if (!checkSumOK) {
                checkSumOK = true;
                operatorMenu.loadDefaultFromFlash();
            }
            initTime = Time::getCurrentTime();
            return initSystem;
        }

        // Check if INIT PROOF is ok then loads all parameters
        // ReSharper disable once CppDFAUnreachableCode
        if (RPU_ReadULFromEEProm(EEPROM_RPU_OS_INIT_PROOF_UL, 0) == RPU_OS_INIT_PROOF) { // CHECKSUM OK
            Serial.println("CHECKSUM OK");
            operatorMenu.readAllEEPROM_Values();
            checkSumOK = true;

            delay(10);
            machineState.setHighScore(static_cast<score_t>(RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_UL)));
            delay(10);

            // Read High score from EEPROM
            machineState.setHighScore(static_cast<score_t>(RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_UL, 0)));

            // Read Last scores from EEPROM
            for (uint8_t count = 0; count < 4; count++) {
                machineState.setScore(static_cast<score_t>(RPU_ReadULFromEEProm(RPU_PLAYER_1_SCORE_UL + 4 * count, 0)), count);
            }
        } else { // INCORRECT CHECKSUM
            Serial.println("WRITE ALL PARAMETERS");
            RPU_WriteULToEEProm(EEPROM_RPU_OS_INIT_PROOF_UL, RPU_OS_INIT_PROOF);
            operatorMenu.writeAllDefaultEEPROM_Values();
            checkSumOK = false;

            // Reset last scores from EEPROM
            for (uint8_t count = 0; count < 4; count++) {
                delay(10);
                RPU_WriteULToEEProm(RPU_PLAYER_1_SCORE_UL + 4 * count, 0);
            }
        }
    }

    if ((!checkSumOK && RPU_ReadSingleSwitchState(SW_SELF_TEST_SWITCH))
        || (checkSumOK && Time::getCurrentTime() - initTime > (DEV_MODE ? 250 : 3000))) return runAttract;

    return initSystem;
}
GameState runOperatorMenu() {
    operatorMenu.run(GameState::hasChanged());

    if (operatorMenu.isExit()) {
        Log::printMessage(Log::LOG_OPERATOR, "Exiting Operator Menu");
        return initSystem;
    }
    return runOperatorMenu;
}
GameState runAttract() {
    if (GameState::hasChanged()) Log::printMessage(Log::LOG_GAMESTATE, "Entering Attract Mode");
    Attract::run(GameState::hasChanged(), machineState);
    return runAttract;
}
GameState initNewGame() {
    static Time::time_t gameStartTime = 0;

    if (GameState::hasChanged()) {
        Log::printMessage(Log::LOG_GAMESTATE, "Init New Game");
        gameStartTime = Time::getCurrentTime();

        machineState.resetGame();
        for (auto& player : playerStates) {
            player.reset();
        }

        DisplayHelper::stopAllDisplayOverride();
        LampsHelper::hideAllPlayfieldLamps();

        SoundHelper::playSoundEffect(DASH51_ADD_PLAYER, AUDIO_DASH51);
    }

    if (Time::getCurrentTime() - gameStartTime < RESTART_GAME_DURATION) return initNewGame;
    machineState.initNewGame();
    return initNewBall;
}
GameState initNewBall() {
    static Time::time_t ballStartTime = 0;

    if (GameState::hasChanged()) {
        Log::printMessage(Log::LOG_GAMESTATE, "Init New Ball");
        ballStartTime = Time::getCurrentTime();

        machineState.initNewBall();
        playerStates[machineState.getCurrentPlayerNumber()].initNewBall();

        machineState.DTB[0]->resetBank(Time::getCurrentTime());
        machineState.DTB[1]->resetBank(Time::getCurrentTime() + 150);
        machineState.DTB[2]->resetBank(Time::getCurrentTime() + 300);

        if (GS_EEPROM::soundType == SOUND_NOISE_WITH_BACKGROUND) SoundHelper::playSoundEffect(DASH51_BACKGROUND_START, AUDIO_DASH51);
    }

    if (Time::getCurrentTime() - ballStartTime < INIT_NEW_BALL_DURATION) return initNewBall;
    return runGameplay;
}
GameState runGameplay() {
    if (GameState::hasChanged()) {
        Gameplay::manageNewState();
        SoundHelper::playSoundEffect(DASH51_NEW_BALL, AUDIO_DASH51);
    }
    return Gameplay::run() ? runBonusCountDown : runGameplay;
}
GameState runBonusCountDown() {
    if (GameState::hasChanged()) {
        Log::printMessage(Log::LOG_GAMESTATE, "Entering Bonus Count Down");
        SoundHelper::playSoundEffect(DASH51_BACKGROUND_STOP, AUDIO_DASH51);
    }
    return machineState.increaseCurrentPlayerNumber() ? runAttract : initNewBall;
}

void setup() {
#if (DEBUG_MESSAGES)
    Serial.begin(115200);
    Serial.write("Starting\n");
#endif

    // Set the seed for better randomness
    randomSeed(RPU_ReadULFromEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE));

    RPU_SetupGameSwitches(
            NUM_SWITCHES_WITH_TRIGGERS,
            NUM_PRIORITY_SWITCHES_WITH_TRIGGERS,
            solenoidAssociatedSwitches
    );

    // Set up the chips and interrupts
    const unsigned long initResult = RPU_InitializeMPU(RPU_CMD_BOOT_ORIGINAL_IF_NOT_SWITCH_CLOSED | RPU_CMD_BOOT_ORIGINAL_IF_CREDIT_RESET, SW_CREDIT_BUTTON);

    if (initResult & RPU_RET_ORIGINAL_CODE_REQUESTED) {
        // Arduino should hang if original code is running
        while (1) {
            // TODO: sleep();
        }
    }

    Log::printMessage(Log::LOG_NONE, "Initialization Finished : New code running");

    DisplayHelper::initDisplay(machineState);
    Scoring::initScoring(machineState, 5, 1);
    Gameplay::init(&machineState, playerStates);
    SoundHelper::clearSoundQueue();
    SoundHelper::WaveTrigger::init();

    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);
    RPU_SetCoinLockout(true);

    Scoring::setScoringState(true);
    Scoring::setAllSoundState(true);
    Scoring::setTensOfThousandAnimationType(true);
    Scoring::setTimeToWait(STACK_TENS, 116);
    Scoring::setTimeToWait(STACK_HUNDRED, 116);
    Scoring::setTimeToWait(STACK_THOUSAND, 116);
    Scoring::setTimeToWait(STACK_EXTRA, 132);

    Time::updateCurrentTime();

    GameState::currentGameState = initSystem;
    Log::printMessage(Log::LOG_NONE, "Entering Main program Loop\n\r");
}

void update() {
    if (GameState::currentGameState != runOperatorMenu && GameState::currentGameState != initSystem) DisplayHelper::update();

    Animations::update();
    LampsHelper::update();
    RPU_UpdateTimedSolenoidStack(Time::getCurrentTime());

    SoundHelper::updateSoundQueue();
    SoundHelper::WaveTrigger::update();
    SoundHelper::Playlist::update();

    for (auto& dropTargetBank : dropTargetBanks) {
        dropTargetBank->update(Time::getCurrentTime());
    }
}
void handleSwitchHit() {
    const uint8_t switchHit = RPU_PullFirstFromSwitchStack();
    if (switchHit == SWITCH_STACK_EMPTY) return;

    switch (switchHit) {
    case SW_COIN_1:
    case SW_COIN_2:
    case SW_COIN_3:
        if (GameState::currentGameState != runOperatorMenu) machineState.manageCoinDrop(switchHit);
        break;
    case SW_CREDIT_BUTTON:
        if ((GameState::currentGameState == runAttract || GameState::currentGameState == runGameplay)) {
            if (machineState.isEnoughCredit()) {
                if (machineState.getBallInPlay() > 1) {
                    Log::printMessage(Log::LOG_MACHINE_STATE, "BIP > 1 --> Restart game");
                    GameState::currentGameState = initNewGame;
                } else machineState.addPlayer();
            }
        }
        break;
    case SW_SELF_TEST_SWITCH:
        if (GameState::currentGameState != runOperatorMenu) GameState::currentGameState = runOperatorMenu;
        break;
    default:
        break;
    }

    if (GameState::currentGameState == runAttract) Attract::handleSwitchHit(switchHit);
    else if (GameState::currentGameState == runGameplay) Gameplay::handleSwitchHit(switchHit);
    else if (GameState::currentGameState == runOperatorMenu) operatorMenu.handleSwitchHit(switchHit);

    MachineState::setMostRecentSwitchHit(switchHit);
    Time::updateMostRecentSwitchHitTime();
}
void loop() {
    RPU_DataRead(0);
    Time::updateCurrentTime();

    const GameState currentGameState = GameState::currentGameState;
    GameState::currentGameState      = GameState::currentGameState();
    GameState::lastGameState         = currentGameState;

    handleSwitchHit();
    update();

    if (GameState::hasChanged()) {
        Serial.println();
        Log::printMessage(Log::LOG_GAMESTATE, "Game state Changed");
    }
}
