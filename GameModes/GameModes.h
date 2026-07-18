#ifndef GS_GAME_MODES_H
#define GS_GAME_MODES_H

#include "BaseMode.h"
#include "GameSpecific/PinballMachineBase/Constants.h"

namespace GameModes {
void initGameModes(MachineState* _machineState, PlayerState* _players);
void initNewBall();

// Game modes life
void startMode(GameModeIDs GameModeID);
void stopMode(GameModeIDs GameModeID);
void update();
void manageSwitchHit(uint8_t switchHit);

// Getters
bool getGameModeStatus(GameModeIDs GameModeID);

// Mode timers
void    manageModeTimer();
uint8_t getGlobalTimer();
void    setGlobalTimer(uint8_t value);
} // namespace GameModes

/***************************************************************************************************/
//
// UNSTRUCTURED PLAY
//
/***************************************************************************************************/

class UnstructuredPlay final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit UnstructuredPlay(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static UnstructuredPlay unstructuredPlay(false);

/***************************************************************************************************/
//
// SKILL SHOT
//
/***************************************************************************************************/

class SkillShot final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit SkillShot(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static SkillShot skillShot(false);

/***************************************************************************************************/
//
// MAIN WIZARD MODE
//
/***************************************************************************************************/

class WizardMode final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit WizardMode(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static WizardMode wizardMode(false);

/***************************************************************************************************/
//
// GAMEMODE 1
//
/***************************************************************************************************/

class GameMode1 final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit GameMode1(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static GameMode1 gamemode1(false);

/***************************************************************************************************/
//
// GAMEMODE 2
//
/***************************************************************************************************/

class GameMode2 final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit GameMode2(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static GameMode2 gamemode2(false);

/***************************************************************************************************/
//
// GAMEMODE 3
//
/***************************************************************************************************/

class GameMode3 final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit GameMode3(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static GameMode3 gamemode3(false);

/***************************************************************************************************/
//
// GAMEMODE 4
//
/***************************************************************************************************/

class GameMode4 final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit GameMode4(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static GameMode4 gamemode4(false);

/***************************************************************************************************/
//
// GAMEMODE 5
//
/***************************************************************************************************/

class GameMode5 final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit GameMode5(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static GameMode5 gamemode5(false);

/***************************************************************************************************/
//
// GAMEMODE 6
//
/***************************************************************************************************/

class GameMode6 final : public BaseMode {
protected:
    void updateLamps() override;

public:
    explicit GameMode6(const bool useGlobal)
        : BaseMode(useGlobal) {}

    void onStart() override;
    void update() override;
    void handleSwitchHit(uint8_t switchHit) override;
    void onEnd() override;
};
static GameMode6 gamemode6(false);

constexpr BaseMode* gameModes[NUMBER_OF_MODES] = {
        &unstructuredPlay,
        &gamemode1,
        &gamemode2,
        &gamemode3,
        &gamemode4,
        &gamemode5,
        &gamemode6,
        &skillShot,
};

#endif
