#ifndef RPU_OPERATOR_MENU_H
#define RPU_OPERATOR_MENU_H

#include "System/RPU.h"
#include "System/Time.h"

#include <stdint.h>

#define DOUBLE_CLICK_TIME 200

enum ValueType : uint8_t {
    TYPE_BYTE,
    TYPE_LONG,
    TYPE_BOOL
};
struct AuditItem {
    uint16_t  eepromAddress;
    ValueType type;
};
enum AdjMode : uint8_t {
    ADJ_TOGGLE,
    ADJ_RANGE,
    ADJ_STEPPED
};
struct AdjItem {
    uint16_t  eepromAddress;
    ValueType type;
    AdjMode   mode;
    uint32_t  minValue;
    uint32_t  maxValue;
    uint32_t  step;
    uint32_t (*stepFunc)(uint32_t);
    uint32_t defaultValue;
    void*    value;
};

enum MenuMainIDs : uint8_t {
    MENU_STD_TESTS = 0,
    MENU_STD_AUDITS,
    MENU_STD_ADJUSTMENTS,
    MENU_GS_ADJUSTMENTS,
    MENU_NUMBER_OF_LEVELS
};
class OperatorMenu {
    Time::time_t lastEnterSwitchTime = 0;

    uint8_t currentMenu = MENU_STD_TESTS;
    uint8_t subLevel    = 0;

    const uint8_t subMenuSizes[MENU_NUMBER_OF_LEVELS];

    bool inSubMenu      = false;
    bool subMenuChanged = false;
    bool exitRequested  = false;

    void handleNavigationSwitch();
    void handleEnterSwitch();
    void handleBackSwitch();

    void displayMenuLevels() const;

public:
    const uint8_t navigationSwitch = 0;
    const uint8_t enterSwitch      = 0;
    const uint8_t backSwitch       = 0;

    const uint8_t solenoidTestStrength = 0;

    const AuditItem* const standardAuditTable;
    const AdjItem* const   standardAdjustTable;
    const AdjItem* const   specificAdjustTable;

    OperatorMenu(
            uint8_t _navigationSwitch, uint8_t _enterSwitch, uint8_t _backSwitch,
            uint8_t _solenoidTestStrength, uint8_t _standardTestTableSize,
            const AuditItem* _standardAuditTable, uint8_t _standardAuditTableSize,
            const AdjItem* _standardAdjustTable, uint8_t _standardAdjustmentTableSize,
            const AdjItem* _specificAdjustTable, uint8_t _specificAdjustTableSize
    );

    uint8_t        getSubMenu() const { return subLevel; }
    uint8_t        getMenu() const { return currentMenu; }
    const AdjItem* getCurrentAdjustmentItem() const {
        if (currentMenu == MENU_STD_ADJUSTMENTS) return &standardAdjustTable[subLevel];
        if (!subMenuSizes[MENU_GS_ADJUSTMENTS]) return nullptr;
        return &specificAdjustTable[subLevel];
    };

    void setDisplays(const uint32_t value) {
        for (uint8_t i = 0; i < 4; i++) {
            RPU_SetDisplay(i, value, true, 2, false);
        }
    }

    void run(bool gameStateChanged);
    void handleSwitchHit(uint8_t switchHit);

    bool hasSubMenuChanged() const { return subMenuChanged; }
    bool isExit() const { return exitRequested; }

    void loadDefaultFromFlash() const;
    void readAllEEPROM_Values() const;
    void writeAllDefaultEEPROM_Values() const;
};

#endif // RPU_OPERATOR_MENU_H
