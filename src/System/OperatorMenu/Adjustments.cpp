#include "Adjustments.h"

#include "OperatorMenu.h"
#include "System/Log.h"

static OperatorMenu* operatorMenu           = nullptr;
static Time::time_t  lastEnterSwitchHitTime = 0;
static Time::time_t  lastSubMenuChangedTime = 0;
static bool          enterHit               = false;
static bool          resetAdjustment        = false;

#define INITIAL_DELAY 500      // Time before auto-repeat starts
#define FAST_REPEAT_DELAY 3000 // Credit button hold time for fast repeats
#define SLOW_REPEAT_PERIOD 150 // Time between repeated increments
#define FAST_REPEAT_PERIOD 75  // Time between repeated increments


static uint32_t readValueFromRAM(const AdjItem* item) {
    switch (item->type) {

    case TYPE_BOOL:
        return *static_cast<bool*>(item->value);

    case TYPE_BYTE:
        return *static_cast<uint8_t*>(item->value);

    case TYPE_LONG:
        return *static_cast<uint32_t*>(item->value);

    default:
        return 0;
    }
}
static void writeValueToEEPROM(const AdjItem* item, uint32_t value) {
    Log::printVariable(Log::LOG_OPERATOR, "Write ADJ", item->eepromAddress);
    switch (item->type) {

    case TYPE_BOOL:
        RPU_WriteByteToEEProm(item->eepromAddress, value ? 1 : 0);
        break;

    case TYPE_BYTE:
        if (value > 255) value = 255;
        RPU_WriteByteToEEProm(item->eepromAddress, static_cast<uint8_t>(value));
        break;

    case TYPE_LONG:
        RPU_WriteULToEEProm(item->eepromAddress, value);
        break;

    default:
        break;
    }
}
static void writeValueToRAM(const AdjItem* item, uint32_t value) {
    Log::printVariable(Log::LOG_OPERATOR, "Write ADJ", item->eepromAddress);
    switch (item->type) {

    case TYPE_BOOL:
        *static_cast<bool*>(item->value) = value ? true : false;
        break;

    case TYPE_BYTE:
        if (value > 255) value = 255;
        *static_cast<uint8_t*>(item->value) = static_cast<uint8_t>(value);
        break;

    case TYPE_LONG:
        *static_cast<uint32_t*>(item->value) = value;
        break;

    default:
        break;
    }
}
static void incrementValue(const AdjItem* item) {
    uint32_t val = readValueFromRAM(item);

    switch (item->mode) {
    case ADJ_TOGGLE:
        val = !val;
        break;
    case ADJ_RANGE:
        val = (val < item->maxValue) ? val + 1 : item->minValue;
        break;
    case ADJ_STEPPED:
        const uint32_t step = item->step;
        // if (item->stepFunc != nullptr) step = item->stepFunc(val);
        val += step;
        if (val > item->maxValue) val = item->minValue;
        break;
    }

    writeValueToRAM(item, val);
    writeValueToEEPROM(item, val);

    operatorMenu->setDisplays(val);
}
static void handleEnterSwitch(const AdjItem* item) {
    static bool     creditPrevState  = false;
    static uint32_t creditPressTime  = 0;
    static uint32_t creditLastRepeat = 0;

    if (resetAdjustment) {
        resetAdjustment = false;
        writeValueToEEPROM(operatorMenu->getCurrentAdjustmentItem(), operatorMenu->getCurrentAdjustmentItem()->minValue);
        operatorMenu->setDisplays(readValueFromRAM(operatorMenu->getCurrentAdjustmentItem()));
        return;
    }

    if (!enterHit) return;
    const bool pressed = RPU_ReadSingleSwitchState(operatorMenu->enterSwitch);

    if (pressed) {
        if (!creditPrevState) {
            // FIRST PRESS
            creditPressTime  = Time::getCurrentTime();
            creditLastRepeat = Time::getCurrentTime();
            incrementValue(item);
        } else {
            // HOLDING
            const uint32_t holdTime = Time::getCurrentTime() - creditPressTime;

            if (holdTime >= INITIAL_DELAY) {
                if ((Time::getCurrentTime() - creditLastRepeat >= SLOW_REPEAT_PERIOD)
                    || ((Time::getCurrentTime() - creditLastRepeat >= FAST_REPEAT_PERIOD) && holdTime >= FAST_REPEAT_DELAY)) {
                    creditLastRepeat = Time::getCurrentTime();
                    incrementValue(item);
                }
            }
        }
    } else {
        // BUTTON RELEASED → reset tracking
        creditPrevState = false;
        enterHit        = false;
        return;
    }

    creditPrevState = pressed;
}

void Adjustments::init(OperatorMenu* _operatorMenu) {
    operatorMenu = _operatorMenu;
}
void Adjustments::run() {
    if (operatorMenu->hasSubMenuChanged()) {
        lastSubMenuChangedTime = Time::getCurrentTime();
        operatorMenu->setDisplays(readValueFromRAM(operatorMenu->getCurrentAdjustmentItem()));
    }
    if (Time::getCurrentTime() - lastSubMenuChangedTime > 500) handleEnterSwitch(operatorMenu->getCurrentAdjustmentItem());
}
void Adjustments::handleSwitchHit(const uint8_t switchHit) {
    if (Time::getCurrentTime() - lastSubMenuChangedTime < 500) return;
    if (switchHit == operatorMenu->enterSwitch) {
        if (Time::getCurrentTime() - lastEnterSwitchHitTime < DOUBLE_CLICK_TIME) resetAdjustment = true;
        else enterHit = true;
        lastEnterSwitchHitTime = Time::getCurrentTime();
    }
}
