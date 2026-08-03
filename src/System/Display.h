/*********************************************************************


Display Header File
Version : 3.00


*********************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "System/MachineState.h"
#include "System/Time.h"

#include <stdint.h>

/*
  Display subsystem rewrite
  - deterministic display mode selection (one effective mode per-display per-update)
  - per-display timers/seeds
  - small animation engine (bounce, flutter, flyby placeholder)
  - credit / ball-in-play handled similarly to player displays
  - minimal public API
*/

namespace DisplayHelper {

// Public animation types (expandable)
enum DisplayAnimations : uint8_t {
    DISPLAY_ANIMATION_NONE = 0,
    DISPLAY_ANIMATION_BOUNCE,
    DISPLAY_ANIMATION_FLUTTER,
    DISPLAY_ANIMATION_FLYBY
};

// Public render modes (selected internally to avoid ambiguous flags)
enum DisplayMode : uint8_t {
    MODE_HIDDEN = 0,
    MODE_STANDARD,
    MODE_FLASH,
    MODE_DASH,
    MODE_CENTER,
    MODE_ANIMATION,
    MODE_SCROLL
};

// Public API (initialize once and call update() regularly)
void initDisplay(MachineState& machineStateRef); // must be called once on startup
void update();                                   // main periodic call (called from main loop)
void blankAllDisplays();

// Player displays control
void setScoreOverride(uint8_t displayNumber, score_t score, bool blankByMagnitude, uint8_t minDigits);
void setScoreOverrideStatus(uint8_t displayNumber, bool status);
void setDisplayBaseMask(uint8_t displayNumber, uint8_t mask);
void setDisplayMaskByMagnitude(uint8_t displayNumber);

void flashDisplayByMagnitude(uint8_t displayNumber, Time::time_t period, uint8_t minDigits);
void setDisplayFlash(uint8_t displayNumber, bool enabled, Time::time_t periodMs, uint8_t mask);
void setDisplayDash(uint8_t displayNumber, bool enabled, Time::time_t periodMs);
void setDisplayCentered(uint8_t displayNumber, bool enabled);
void setDisplayAnimation(uint8_t displayNumber, DisplayAnimations anim, Time::time_t stepDurationMs, uint8_t animMask);

// Stop overrides
void stopDisplayOverride(uint8_t displayNumber);
void stopAllDisplayOverride();

// Credit / Ball-in-play display
void setCreditDisplayValue(uint8_t value);
void setBallInPlayDisplayValue(uint8_t value);
void setCreditDisplayFlashing(bool flash, Time::time_t periodMs = 300);
void setBallInPlayDisplayFlashing(bool flash, Time::time_t periodMs = 300);

} // namespace DisplayHelper

#endif
