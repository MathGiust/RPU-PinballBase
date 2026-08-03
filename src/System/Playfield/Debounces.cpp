#include "Debounces.h"

#define DEBOUNCE_PERIOD_MS 20

DebouncedSwitch::DebouncedSwitch(const uint8_t _switchIndex)
    : switchIndex(_switchIndex),
      readings(0), lastReadingTime(0),
      status(DEBOUNCE_IDLE) {}

void DebouncedSwitch::startDebounce() {
    status = DEBOUNCE_PENDING;
    readings = 1;
}
void DebouncedSwitch::update() {
    if (status != DEBOUNCE_PENDING) return;
    if (Time::getCurrentTime() - lastReadingTime < DEBOUNCE_PERIOD_MS) return;

    lastReadingTime = Time::getCurrentTime();
    readings = ((readings << 1) | (RPU_ReadSingleSwitchState(switchIndex) & 1));
    if (readings == 0) status = DEBOUNCE_LOW;
    else if (readings == 0xFF) status = DEBOUNCE_HIGH;
}
void DebouncedSwitch::resetStatus() {
    status = DEBOUNCE_IDLE;
}
DebounceStates DebouncedSwitch::getDebounceState() const {
    return status;
}
