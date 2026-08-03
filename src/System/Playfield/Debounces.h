/*********************************************************************


Drop Targets Header File
    Version : 1.10


*********************************************************************/

#ifndef DEB_H
#define DEB_H

#include "System/RPU.h"
#include "System/Time.h"

enum DebounceStates : uint8_t {
    DEBOUNCE_LOW = 0,
    DEBOUNCE_HIGH,
    DEBOUNCE_PENDING,
    DEBOUNCE_IDLE
};
class DebouncedSwitch {
    const uint8_t  switchIndex;
    uint8_t        readings;
    Time::time_t   lastReadingTime;
    DebounceStates status;

public:
    explicit DebouncedSwitch(uint8_t _switchIndex);

    void startDebounce();
    void update();
    void resetStatus();

    DebounceStates getDebounceState() const;
};

#endif
