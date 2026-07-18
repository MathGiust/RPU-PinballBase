#include "AttractState.h"
#include "BonusCountDown.h"
#include "GameModes/GameModes.h"
#include "Gameplay.h"
#include "Lamps.h"
#include "PinballMachineBase/PinballBase.h"
#include "PinballMachineBase/Playfield.h"
#include "PlayerState.h"
#include "RPU_Config.h"
#include "System/Display.h"
#include "System/Log.h"
#include "System/MachineState.h"
#include "System/MatchMode.h"
#include "System/RPU.h"
#include "System/Scoring.h"
#include "System/SelfTestAndAudit.h"
#include "System/Sound/Sound.h"
#include "System/Sound/WaveTriggerHandler.h"
#include "System/Time.h"

#include <Arduino.h>

PlayfieldAndCabinetSwitch solenoidAssociatedSwitches[] = {
        {SW_RIGHT_SLING,   SOL_RIGHT_SLING,   SOL_SLING_STRENGTH },
        {SW_LEFT_SLING,    SOL_LEFT_SLING,    SOL_SLING_STRENGTH },
        {SW_LEFT_BUMPER,   SOL_LEFT_BUMPER,   SOL_BUMPER_STRENGTH},
        {SW_RIGHT_BUMPER,  SOL_RIGHT_BUMPER,  SOL_BUMPER_STRENGTH},
        {SW_BOTTOM_BUMPER, SOL_BOTTOM_BUMPER, SOL_BUMPER_STRENGTH}
};

PlayerState playerStates[4];
score_t*    scores[4]{
        &playerStates[0].score,
        &playerStates[1].score,
        &playerStates[2].score,
        &playerStates[3].score,
};

DropTargetBankControlled* dropTargetBanks[NUMBER_OF_DROP_TARGET_BANKS]{};
StandupTargetBank*        standupTargetBanks[NUMBER_OF_STANDUP_TARGET_BANKS]{};
DebouncedSwitch*          debouncedSwitches[NUMBER_OF_DEBOUNCED_SWITCHES]{};

MachineState machineState(
        4, scores,
        NUMBER_OF_DROP_TARGET_BANKS, *dropTargetBanks,
        NUMBER_OF_STANDUP_TARGET_BANKS, *standupTargetBanks,
        NUMBER_OF_DEBOUNCED_SWITCHES, *debouncedSwitches,
        SW_OUTHOLE
);

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
    unsigned long initResult = RPU_InitializeMPU(RPU_CMD_BOOT_ORIGINAL_IF_NOT_SWITCH_CLOSED | RPU_CMD_BOOT_ORIGINAL_IF_CREDIT_RESET, SW_CREDIT_BUTTON);

    if (initResult & RPU_RET_ORIGINAL_CODE_REQUESTED) {
        // Arduino should hang if original code is running
        while (1) {
        }
        // TODO: sleep();
    }

    Log::printMessage(Log::LOG_NONE, "Initialization Finished : New code running\n\r");

    DisplayHelper::initDisplay(machineState);
    Scoring::initScoring(machineState);
    Gameplay::initGameplay(&machineState, playerStates);
    GameModes::initGameModes(&machineState, playerStates);
    SoundHelper::clearSoundQueue();
    SoundHelper::WaveTrigger::init();

    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);
    RPU_SetCoinLockout(true);

    constexpr ScoringStackIDs priority[] = {STACK_FAST_THOUSAND, STACK_BONUS, STACK_TENS, STACK_HUNDRED, STACK_THOUSAND, STACK_EXTRA};
    Scoring::setPriority(priority);
    Scoring::setScoringState(true);
    Scoring::setAllSoundState(false);
    Scoring::setTensOfThousandAnimationType(true);
    Scoring::setSoundState(STACK_EXTRA, true);
    Scoring::setSoundState(STACK_BONUS, true);
    Scoring::setSoundState(STACK_THOUSAND, false);

    Scoring::setTimeToWait(STACK_BONUS, 260);
    Scoring::setTimeToWait(STACK_EXTRA, 200);
    Scoring::setTimeToWait(STACK_THOUSAND, 120);
    Scoring::setTimeToWait(STACK_HUNDRED, 120);
    Scoring::setTimeToWait(STACK_TENS, 120);

    Time::updateCurrentTime();
}

void handleStateMachine(int currentState, const boolean machineStateChanged) {
    switch (currentState) {
    default:
        currentState = SelfTestAndAudit::run(currentState, machineStateChanged);
        break;
    case MACHINE_STATE_INITIALIZE:
        currentState = machineState.runInitState(machineStateChanged);
        break;
    case MACHINE_STATE_ATTRACT:
        currentState = Attract::run(machineStateChanged, machineState);
        break;
    case MACHINE_STATE_RESTART_GAME:
        currentState = machineState.restartGame(machineStateChanged);
        break;
    case MACHINE_STATE_INIT_GAMEPLAY:
        currentState = machineState.initGamePlay();
        break;
    case MACHINE_STATE_INIT_NEW_BALL:
        currentState = machineState.initNewBall(machineStateChanged);
        break;
    case MACHINE_STATE_NORMAL_GAMEPLAY:
        currentState = Gameplay::run(machineStateChanged);
        break;
    case MACHINE_STATE_COUNTDOWN_BONUS:
        currentState = BonusCountDown::run(machineStateChanged, &playerStates[machineState.getCurrentPlayerNumber()]);
        break;
    case MACHINE_STATE_MATCH_MODE:
        currentState = MatchMode::run(machineStateChanged);
        break;
    case MACHINE_STATE_BALL_OVER:
        if (machineState.getSamePlayerShootsAgain()) currentState = MACHINE_STATE_INIT_NEW_BALL;
        else currentState = machineState.increaseCurrentPlayerNumber();
        break;
    }

    machineState.setMachineState(currentState); // Set new machine state
}
void update(const int currentState) {
    if (currentState > 0) DisplayHelper::update();

    LampsHelper::update();
    RPU_UpdateTimedSolenoidStack(Time::getCurrentTime());

    SoundHelper::updateSoundQueue();
    SoundHelper::WaveTrigger::update();
    SoundHelper::Playlist::update();

    machineState.DTB->update();
    machineState.DEB->update();
}
void loop() {
    RPU_DataRead(0);
    Time::updateCurrentTime();

    int           currentState        = machineState.getMachineState();
    const boolean machineStateChanged = machineState.getMachineStateChanged();

    handleStateMachine(currentState, machineStateChanged);
    update(currentState);
}
