#ifndef BASEMODE_H
#define BASEMODE_H

#include "GameSpecific/PlayerState.h"
#include "System/MachineState.h"

#include <stdint.h>

class BaseMode {
protected:
    bool    active;
    bool    useGlobalTimer;
    bool    lampsNeedUpdate;
    uint8_t modeTimer;

    static uint8_t       globalTimer; // Shared across all modes
    static PlayerState*  players;
    static PlayerState*  currentPlayer;
    static MachineState* machineState;

    virtual void updateLamps() {}

public:
    explicit BaseMode(const bool useGlobal)
        : active(false), useGlobalTimer(useGlobal), lampsNeedUpdate(false), modeTimer(0) {}

    virtual ~BaseMode() = default;

    // ---- Static variables ----
    static void initBaseMode(MachineState* _machineState, PlayerState* _players) {
        machineState = _machineState;
        players      = _players;
    }
    static void updateCurrentPlayer() { currentPlayer = &players[machineState->getCurrentPlayerNumber()]; }

    // ---- Lifecycle ----
    virtual void onStart() {}
    virtual void update() {}
    virtual void handleSwitchHit(uint8_t switchHit) {}
    virtual void onEnd() {}

    // ---- State Access ----
    bool isActive() const { return active; }
    void setActive(const bool val) { active = val; }

    // ---- Timer Handling ----
    uint8_t getTimer() const {
        return useGlobalTimer ? globalTimer : modeTimer;
    }

    void setTimer(const uint8_t value) {
        if (useGlobalTimer) globalTimer = value;
        else modeTimer = value;
    }

    void incrementTimer() {
        if (useGlobalTimer) globalTimer++;
        else modeTimer++;
    }

    bool isGlobalTimerUsed() const { return useGlobalTimer; }

    // ---- Static global timer ----
    static void    setGlobalTimer(const uint8_t value) { globalTimer = value; }
    static uint8_t getGlobalTimer() { return globalTimer; }
};

#endif // BASEMODE_H
