/*********************************************************************


Drop Targets Header File
    Version : 1.10


*********************************************************************/

#ifndef DTB_H
#define DTB_H

#include "System/RPU.h"
#include "System/Time.h"

class DropTargetBank final {
    const uint8_t numberOfTargets;
    const uint8_t numberOfResetSolenoids;
    const uint8_t resetSolenoidStrength;

    // Arrays
    const uint8_t* const switchArray;
    const uint8_t* const resetSolenoidArray;
    bool* const          targetHitArray;

    // Variables
    bool         bankResetting;
    Time::time_t bankResetTime;
    Time::time_t targetResetEndTime;
    Time::time_t lastResetTime;

public:
    DropTargetBank(
            uint8_t        _numberOfTargets,
            const uint8_t* _switchArray,
            uint8_t        _numberOfResetSolenoids,
            const uint8_t* _resetSolenoidArray,
            uint8_t        _resetSolenoidStrength,
            bool*          _targetHitArray
    );
    ~DropTargetBank() = default;

    void update(Time::time_t currentTime);
    void resetBank(Time::time_t whenToReset);
    void readBank() const;

    bool handleTargetHit(uint8_t targetHit) const;
    bool checkIfBankCleared() const;
    bool isSwitchHitInBank(uint8_t switchIndex) const;

    bool         isTargetHit(const uint8_t targetIndex) const { return targetHitArray[targetIndex]; }
    bool         isBankResetting() const { return bankResetting; }
    bool*        getTargetHitArray() const { return targetHitArray; }
    uint8_t      getNumberOfTargets() const { return numberOfTargets; }
    Time::time_t getTargetResetEndTime() const { return targetResetEndTime; }
    Time::time_t getLastResetTime() const { return lastResetTime; }

    uint8_t getTargetHitIndex(uint8_t switchHit) const;
};

#endif
