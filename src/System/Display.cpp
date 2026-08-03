/*********************************************************************


Display Source File
Version : 3.00


*********************************************************************/

#include "Display.h"

#include "RPU.h"

#include <string.h> // for memset

// ---------- Configurable defaults ----------
static constexpr Time::time_t DEFAULT_FLASH_PERIOD_MS = 250;
static constexpr Time::time_t MIN_STEP_MS = 20;

// ---------- Internal structures ----------
struct DisplayData {
    // Source
    score_t* playerScorePtr = nullptr;

    // Override
    score_t overrideScore = 0;
    bool    overrideStatus = false;

    // Base rendering
    bool    wantCentered = false;
    uint8_t baseMask = RPU_OS_ALL_DIGITS_MASK; // always applied

    // Flash
    bool         wantFlash = false;
    Time::time_t flashPeriod = 250;
    uint8_t      flashMask = RPU_OS_ALL_DIGITS_MASK;
    uint32_t     lastFlashSeed = 0;

    // Dash
    bool         wantDash = false;
    Time::time_t dashPeriod = 80;
    uint32_t     lastDashSeed = 0;

    // Animation
    DisplayHelper::DisplayAnimations currentAnimation = DisplayHelper::DISPLAY_ANIMATION_NONE;
    Time::time_t                     animStepDuration = 250;
    Time::time_t                     animStartTime = 0;
    uint32_t                         lastAnimSeed = 0;
    uint8_t                          animMask = RPU_OS_ALL_DIGITS_MASK;

    // Scroll
    uint8_t lastScrollPhase = 0;
};

struct CenterDisplay {
    uint8_t      displayValue = 0;
    bool         flashing = false;
    Time::time_t flashPeriod = DEFAULT_FLASH_PERIOD_MS;
    uint32_t     lastSeed = 0;
};

static MachineState* machineState = nullptr;
static DisplayData   displays[4];
static CenterDisplay centerDisplays[2]; // 0: credits, 1: ball-in-play

// ---------- Helper prototypes ----------
static uint8_t getDisplayMask(uint8_t numDigits);
static uint8_t magnitudeOfScore(score_t score);
static uint8_t numberOfDigits(score_t score);
static void    renderSingleDisplay(uint8_t displayNumber);

static void renderStandard(uint8_t displayNumber, score_t score);
static void renderFlash(uint8_t displayNumber, score_t score);
static void renderDash(uint8_t displayNumber, score_t score);
static void renderCenter(uint8_t displayNumber, score_t score);
static void renderScroll(uint8_t displayNumber, score_t score);
static void renderAnimation(uint8_t displayNumber, score_t score);
static void updateCenterDisplays();

static DisplayHelper::DisplayMode chooseDisplayMode(uint8_t displayNumber, score_t score);

// ---------- Utility ----------
static uint8_t getDisplayMask(const uint8_t numDigits) {
    uint8_t displayMask = 0;
    for (uint8_t digitCount = 0; digitCount < numDigits; digitCount++) {
#ifdef RPU_OS_USE_7_DIGIT_DISPLAYS
        displayMask |= (0x40 >> digitCount);
#else
        displayMask |= (0x20 >> digitCount);
#endif
    }
    return displayMask;
}
static uint8_t magnitudeOfScore(score_t score) {
    if (score == 0) return 0;
    uint8_t mag = 0;
    while (score > 0) {
        score /= 10;
        ++mag;
    }
    return mag;
}
static uint8_t numberOfDigits(const score_t score) {
    if (score == 0) return 1;
    return magnitudeOfScore(score);
}
static uint8_t computeMagnitudeMask(const score_t value, const uint8_t minDigits) {
    uint8_t digits = magnitudeOfScore(value);
    if (digits < minDigits)
        digits = minDigits;

    return getDisplayMask(digits);
}

// ---------- Public API implementation ----------
void DisplayHelper::initDisplay(MachineState& machineStateRef) {
    machineState = &machineStateRef;
    // Reset arrays
    memset(displays, 0, sizeof(displays));
    memset(centerDisplays, 0, sizeof(centerDisplays));

    // hook player score pointers
    for (uint8_t i = 0; i < 4; ++i) {
        displays[i].playerScorePtr = machineState->getPlayerScorePtr(i);
        // sensible initial masks
        displays[i].flashMask = RPU_OS_ALL_DIGITS_MASK;
    }
    // initialize center displays defaults
    centerDisplays[0].displayValue = 0;
    centerDisplays[1].displayValue = 0;
}
void DisplayHelper::update() {
    if (!machineState) return;

    // Update center displays first (they use a separate render)
    updateCenterDisplays();

    // Update player displays
    for (uint8_t displayNumber = 0; displayNumber < 4; ++displayNumber) {
        renderSingleDisplay(displayNumber);
    }
}
void DisplayHelper::blankAllDisplays() {
    for (uint8_t i = 0; i < 4; i++) {
        RPU_SetDisplayBlank(i, 0);
    }
}

// Player displays API
void DisplayHelper::setScoreOverride(const uint8_t displayNumber, const score_t score, const bool blankByMagnitude, const uint8_t minDigits) {
    if (displayNumber >= 4) return;
    displays[displayNumber].overrideScore = score;
    displays[displayNumber].overrideStatus = true;
    if (blankByMagnitude) displays[displayNumber].baseMask = computeMagnitudeMask(score, minDigits);
}
void DisplayHelper::setScoreOverrideStatus(const uint8_t displayNumber, const bool status) {
    if (displayNumber >= 4) return;
    displays[displayNumber].overrideStatus = status;
}
void DisplayHelper::setDisplayBaseMask(const uint8_t displayNumber, const uint8_t mask) {
    displays[displayNumber].baseMask = mask;
}
void DisplayHelper::setDisplayMaskByMagnitude(const uint8_t displayNumber) {
    DisplayData& d = displays[displayNumber];

    score_t value = d.overrideStatus ? d.overrideScore : (d.playerScorePtr ? *d.playerScorePtr : 0);

    // Structural mask: limit visible digits permanently
    d.baseMask = computeMagnitudeMask(value, 2);
}

void DisplayHelper::flashDisplayByMagnitude(const uint8_t displayNumber, const Time::time_t period, const uint8_t minDigits) {
    DisplayData& d = displays[displayNumber];

    score_t value = d.overrideStatus ? d.overrideScore : (d.playerScorePtr ? *d.playerScorePtr : 0);

    // Structural mask: limit visible digits permanently
    d.baseMask = computeMagnitudeMask(value, minDigits);

    // Temporal mask: fully blank during OFF phase
    d.flashMask = 0x00;

    d.flashPeriod = period;
    d.wantFlash = true;
    d.lastFlashSeed = 0;
}

void DisplayHelper::setDisplayFlash(const uint8_t displayNumber, const bool enabled, Time::time_t periodMs, const uint8_t mask) {
    if (displayNumber >= 4) return;
    displays[displayNumber].wantFlash = enabled;
    if (periodMs < MIN_STEP_MS) periodMs = MIN_STEP_MS;
    displays[displayNumber].flashPeriod = periodMs;
    displays[displayNumber].flashMask = mask;
}
void DisplayHelper::setDisplayDash(const uint8_t displayNumber, const bool enabled, Time::time_t periodMs) {
    if (displayNumber >= 4) return;
    displays[displayNumber].wantDash = enabled;
    if (periodMs < MIN_STEP_MS) periodMs = MIN_STEP_MS;
    displays[displayNumber].dashPeriod = periodMs;
}
void DisplayHelper::setDisplayCentered(const uint8_t displayNumber, const bool enabled) {
    if (displayNumber >= 4) return;
    displays[displayNumber].wantCentered = enabled;
}
void DisplayHelper::setDisplayAnimation(const uint8_t displayNumber, const DisplayAnimations anim, Time::time_t stepDurationMs, const uint8_t animMask) {
    if (displayNumber >= 4) return;
    displays[displayNumber].currentAnimation = anim;
    if (stepDurationMs < MIN_STEP_MS) stepDurationMs = MIN_STEP_MS;
    displays[displayNumber].animStepDuration = stepDurationMs;
    displays[displayNumber].animMask = animMask;
    displays[displayNumber].animStartTime = Time::getCurrentTime();
    // animation intentionally takes priority over wantFlash/wantDash/wantCentered
}

// Stop overrides
void DisplayHelper::stopDisplayOverride(const uint8_t displayNumber) {
    if (displayNumber >= 4) return;
    displays[displayNumber].overrideStatus = false;
    displays[displayNumber].overrideScore = 0;
    displays[displayNumber].wantCentered = false;
    displays[displayNumber].wantFlash = false;
    displays[displayNumber].wantDash = false;
    displays[displayNumber].currentAnimation = DisplayHelper::DISPLAY_ANIMATION_NONE;
    displays[displayNumber].animMask = 0xFF;
}
void DisplayHelper::stopAllDisplayOverride() {
    for (uint8_t i = 0; i < 4; ++i)
        stopDisplayOverride(i);
    // center displays stop flashing
    centerDisplays[0].flashing = false;
    centerDisplays[1].flashing = false;
}

// Credit / BIP
void DisplayHelper::setCreditDisplayValue(const uint8_t value) {
    centerDisplays[0].displayValue = value;
}
void DisplayHelper::setBallInPlayDisplayValue(const uint8_t value) {
    centerDisplays[1].displayValue = value;
}
void DisplayHelper::setCreditDisplayFlashing(const bool flash, Time::time_t periodMs) {
    centerDisplays[0].flashing = flash;
    if (periodMs < MIN_STEP_MS) periodMs = MIN_STEP_MS;
    centerDisplays[0].flashPeriod = periodMs;
}
void DisplayHelper::setBallInPlayDisplayFlashing(const bool flash, Time::time_t periodMs) {
    centerDisplays[1].flashing = flash;
    if (periodMs < MIN_STEP_MS) periodMs = MIN_STEP_MS;
    centerDisplays[1].flashPeriod = periodMs;
}

// ---------- Internal renderer ----------
static DisplayHelper::DisplayMode chooseDisplayMode(const uint8_t displayNumber, const score_t score) {
    if (displayNumber >= machineState->getNumberOfPlayers() && !displays[displayNumber].overrideStatus) return DisplayHelper::MODE_HIDDEN;
    if (displays[displayNumber].currentAnimation != DisplayHelper::DISPLAY_ANIMATION_NONE) return DisplayHelper::MODE_ANIMATION;
    if (score > RPU_OS_MAX_DISPLAY_SCORE) return DisplayHelper::MODE_SCROLL;
    if (displays[displayNumber].wantFlash) return DisplayHelper::MODE_FLASH;
    if (displays[displayNumber].wantDash) return DisplayHelper::MODE_DASH;
    if (displays[displayNumber].wantCentered) return DisplayHelper::MODE_CENTER;
    return DisplayHelper::MODE_STANDARD;
}
static void applyDisplay(const uint8_t displayNumber, const score_t score, const uint8_t temporalMask, const bool blankByMagnitude, const uint8_t minDigits) {
    const uint8_t finalMask = displays[displayNumber].baseMask & temporalMask;
    RPU_SetDisplay(displayNumber, score, blankByMagnitude, minDigits, false);
    RPU_SetDisplayBlank(displayNumber, finalMask);
}
static void renderSingleDisplay(const uint8_t displayNumber) {
    const score_t scoreToDisplay = displays[displayNumber].overrideStatus ? displays[displayNumber].overrideScore : *displays[displayNumber].playerScorePtr;

    const DisplayHelper::DisplayMode mode = chooseDisplayMode(displayNumber, scoreToDisplay);

    switch (mode) {
    case DisplayHelper::MODE_HIDDEN:
        RPU_SetDisplayBlank(displayNumber, 0x00);
        break;
    case DisplayHelper::MODE_ANIMATION:
        renderAnimation(displayNumber, scoreToDisplay);
        break;
    case DisplayHelper::MODE_SCROLL:
        renderScroll(displayNumber, scoreToDisplay);
        break;
    case DisplayHelper::MODE_FLASH:
        renderFlash(displayNumber, scoreToDisplay);
        break;
    case DisplayHelper::MODE_DASH:
        renderDash(displayNumber, scoreToDisplay);
        break;
    case DisplayHelper::MODE_CENTER:
        renderCenter(displayNumber, scoreToDisplay);
        break;
    case DisplayHelper::MODE_STANDARD:
    default:
        renderStandard(displayNumber, scoreToDisplay);
        break;
    }
}

// Render implementations
static void renderStandard(const uint8_t displayNumber, const score_t score) {
    RPU_SetDisplay(displayNumber, score, true, 2, false);
}
static void renderFlash(const uint8_t displayNumber, const score_t score) {
    // Re-compute flash mask only if no override
    if (!displays[displayNumber].overrideStatus) {
        displays[displayNumber].baseMask = computeMagnitudeMask(score, 2);
    }

    const uint32_t seed = Time::getCurrentTime() / displays[displayNumber].flashPeriod;
    if (seed != displays[displayNumber].lastFlashSeed) displays[displayNumber].lastFlashSeed = seed;
    const uint8_t temporalMask = (seed & 1) ? RPU_OS_ALL_DIGITS_MASK : displays[displayNumber].flashMask;
    applyDisplay(displayNumber, score, temporalMask, false, 0);
}
static void renderDash(const uint8_t displayNumber, const score_t score) {
    // Sweep dashes across digits (similar to previous algorithm), using dashPeriod
    const uint32_t seed = Time::getCurrentTime() / (displays[displayNumber].dashPeriod ? displays[displayNumber].dashPeriod : 80);
    if (seed == displays[displayNumber].lastDashSeed) return;
    displays[displayNumber].lastDashSeed = seed;

    const uint8_t numDigits = magnitudeOfScore(score);
    const uint8_t phase = (Time::getCurrentTime() / displays[displayNumber].dashPeriod) % (5 * RPU_OS_NUM_DIGITS); // keep previous phase math
    if (phase < (2 * RPU_OS_NUM_DIGITS)) {
        uint8_t displayMask = 0x7F;
        displayMask = getDisplayMask((numDigits == 0) ? 2 : numDigits);
        if (phase < RPU_OS_NUM_DIGITS + 1) {
            for (uint8_t m = 0; m < phase; ++m)
                displayMask &= ~(0x01 << m);
        } else {
            for (uint8_t m = 2 * RPU_OS_NUM_DIGITS; m > phase; --m) {
                constexpr uint8_t firstDigit = (0x20) << (RPU_OS_NUM_DIGITS - 6);
                displayMask &= ~(firstDigit >> (m - phase - 1));
            }
        }
        RPU_SetDisplay(displayNumber, score);
        RPU_SetDisplayBlank(displayNumber, displayMask);
    } else {
        RPU_SetDisplay(displayNumber, score, true, 2, true);
    }
}
static void renderCenter(const uint8_t displayNumber, score_t score) {
    const uint8_t numDigits = magnitudeOfScore(score);
    const uint8_t shift = (RPU_OS_NUM_DIGITS - numDigits) / 2;

    uint8_t mask = getDisplayMask(numDigits);
    for (uint8_t i = 0; i < shift; i++) {
        score *= 10;
        mask >>= 1;
    }

    displays[displayNumber].baseMask = mask;
    applyDisplay(displayNumber, score, RPU_OS_ALL_DIGITS_MASK, false, 1);
}
static void renderScroll(const uint8_t displayNumber, score_t score) {
    // Scrolling uses global "time since last score change" rule (copied semantics)
    const Time::time_t timeSinceLastScoreChange = Time::getCurrentTime() - Time::getMostRecentSwitchHitTime();
    const Time::time_t scrollPhase = (timeSinceLastScoreChange / 150) % 16;
    if (scrollPhase != displays[displayNumber].lastScrollPhase) {
        displays[displayNumber].lastScrollPhase = (uint8_t)scrollPhase;
    }

    if (timeSinceLastScoreChange < 4000) {
        RPU_SetDisplay(displayNumber, (score % (RPU_OS_MAX_DISPLAY_SCORE + 1)), false);
        RPU_SetDisplayBlank(displayNumber, RPU_OS_ALL_DIGITS_MASK);
        return;
    }

    if (scrollPhase < 11) {
        const uint8_t numDigits = magnitudeOfScore(score);
        score_t       tempScore = score;
        uint8_t       displayMask;

        if (scrollPhase < RPU_OS_NUM_DIGITS) {
            displayMask = RPU_OS_ALL_DIGITS_MASK;
            for (uint8_t sc = 0; sc < scrollPhase; ++sc) {
                score = (score % (RPU_OS_MAX_DISPLAY_SCORE + 1)) * 10;
                displayMask = displayMask >> 1;
            }
        } else {
            score = 0;
            displayMask = 0x00;
        }

        if ((numDigits + scrollPhase) > 10) {
            const uint8_t numDigitsNeeded = (numDigits + scrollPhase) - 10;
            for (uint8_t sc = 0; sc < (numDigits - numDigitsNeeded); ++sc) {
                tempScore /= 10;
            }
            displayMask |= getDisplayMask(magnitudeOfScore(tempScore));
            score += tempScore;
        }

        RPU_SetDisplayBlank(displayNumber, displayMask);
        RPU_SetDisplay(displayNumber, score);
    }
}

// Animation rendering (bounce + flutter implemented)
static void renderFlutter(const uint8_t displayNumber, const score_t score) {
    const uint32_t step = (Time::getCurrentTime() - displays[displayNumber].animStartTime) / displays[displayNumber].animStepDuration;
    const uint8_t  temporalMask = (step & 1) ? 0x55 : 0xAA;
    applyDisplay(displayNumber, score, temporalMask, false, 0);
}
static void renderAnimation(const uint8_t displayNumber, const score_t score) {
    const DisplayHelper::DisplayAnimations anim = displays[displayNumber].currentAnimation;
    switch (anim) {
    case DisplayHelper::DISPLAY_ANIMATION_BOUNCE: {
        // Bounce left-right across digits
        const uint8_t  numDigits = magnitudeOfScore(score);
        const uint32_t tick = Time::getCurrentTime() / displays[displayNumber].animStepDuration;
        if (tick == displays[displayNumber].lastAnimSeed) return;
        displays[displayNumber].lastAnimSeed = tick;

        uint8_t shift = (tick) % (2 * (RPU_OS_NUM_DIGITS - numDigits));
        if (shift >= (RPU_OS_NUM_DIGITS + 1 - numDigits)) {
            shift = (RPU_OS_NUM_DIGITS - numDigits) * 2 - shift;
        }

        uint8_t mask = getDisplayMask(numDigits);
        score_t s = score;
        for (uint8_t i = 0; i < shift; ++i) {
            s *= 10;
            mask >>= 1;
        }
        RPU_SetDisplay(displayNumber, s, false);
        RPU_SetDisplayBlank(displayNumber, mask);
        break;
    }
    case DisplayHelper::DISPLAY_ANIMATION_FLUTTER: {
        renderFlutter(displayNumber, score);
        break;
    }
    case DisplayHelper::DISPLAY_ANIMATION_FLYBY:
    default:
        // fallback to standard for now
        RPU_SetDisplay(displayNumber, score, true, 2, false);
        break;
    }
}

// Center displays update
static void updateCenterDisplays() {
    // bip: index 1, credits: index 0 (matching prior naming)
    // credits
    {
        CenterDisplay& cd = centerDisplays[0];
        const uint32_t seed = Time::getCurrentTime() / (cd.flashPeriod ? cd.flashPeriod : DEFAULT_FLASH_PERIOD_MS);
        if (!cd.flashing) {
            RPU_SetDisplayCredits(cd.displayValue, true, true);
        } else {
            // toggle every step
            if (seed != cd.lastSeed) {
                cd.lastSeed = seed;
                if ((seed & 1) == 0) RPU_SetDisplayCredits(cd.displayValue, false);
                else RPU_SetDisplayCredits(cd.displayValue, true);
            }
        }
    }
    // ball-in-play
    {
        CenterDisplay& bd = centerDisplays[1];
        const uint32_t seed = Time::getCurrentTime() / (bd.flashPeriod ? bd.flashPeriod : DEFAULT_FLASH_PERIOD_MS);
        if (!bd.flashing) {
            RPU_SetDisplayBallInPlay(bd.displayValue, true, false);
        } else {
            if (seed != bd.lastSeed) {
                bd.lastSeed = seed;
                if ((seed & 1) == 0) RPU_SetDisplayBallInPlay(bd.displayValue, false);
                else RPU_SetDisplayBallInPlay(bd.displayValue, true, false);
            }
        }
    }
}
