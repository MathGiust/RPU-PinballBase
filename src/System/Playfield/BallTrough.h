/*********************************************************************


Drop Targets Header File
    Version : 1.10


*********************************************************************/

#ifndef BT_H
#define BT_H

#include "Debounces.h"

class BallTrough {

    // Trough
    const uint8_t   ballTroughSize;
    const uint8_t*  ballTroughSwitches;
    DebouncedSwitch* const ballTroughDebounce;
    bool*           ballsInTroughStatus;

    // Locks
    const uint8_t   ballLockSize;
    const uint8_t*  ballLockSwitches;
    DebouncedSwitch* const ballLockDebounce;
    bool*           ballsInLockStatus;

    // Outhole
    const uint8_t outholeSwitch;
    DebouncedSwitch      outholeDebounce;
    bool          outholeStatus;

public:
    BallTrough(
            uint8_t _ballTroughSize, const uint8_t* _ballTroughSwitches,
            uint8_t _ballLockSize, const uint8_t* _ballLockSwitches,
            uint8_t _outholeSwitch
    );
    ~BallTrough();

    void update();
    void startDebounce();
    void resetDebounce();

    bool isDebouncePending() const;
    bool isDebounceIdle() const;
    bool getTroughStatus(uint8_t index) const;
    bool getBallLockStatus(uint8_t index) const;
    bool getOutholeStatus() const;

    uint8_t getCurrentNumbersOfBallsInThrough() const;
    uint8_t getCurrentNumbersOfBallsLocked() const;
};
#endif
