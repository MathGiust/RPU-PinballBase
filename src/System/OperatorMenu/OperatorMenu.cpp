#include "OperatorMenu.h"

#include "Adjustments.h"
#include "Audits.h"
#include "SelfTests.h"
#include "System/Display.h"
#include "System/Lamps/LampsHelper.h"
#include "System/Log.h"
#include "System/RPU.h"

OperatorMenu::OperatorMenu(
        const uint8_t _navigationSwitch, const uint8_t _enterSwitch, const uint8_t _backSwitch,
        const uint8_t _solenoidTestStrength, const uint8_t _standardTestTableSize,
        const AuditItem* _standardAuditTable, const uint8_t _standardAuditTableSize,
        const AdjItem* _standardAdjustTable, const uint8_t _standardAdjustmentTableSize,
        const AdjItem* _specificAdjustTable, const uint8_t _specificAdjustTableSize
) : subMenuSizes{_standardTestTableSize, _standardAuditTableSize, _standardAdjustmentTableSize, _specificAdjustTableSize},
    navigationSwitch(_navigationSwitch), enterSwitch(_enterSwitch), backSwitch(_backSwitch), solenoidTestStrength(_solenoidTestStrength),
    standardAuditTable(_standardAuditTable), standardAdjustTable(_standardAdjustTable), specificAdjustTable(_specificAdjustTable) {
    SelfTests::init(this);
    Audits::init(this);
    Adjustments::init(this);
}

void OperatorMenu::handleNavigationSwitch() {
    // Log::printMessage(Log::LOG_OPERATOR, "Navigation switch pressed");
    if (inSubMenu) {
        subMenuChanged = true;
        subLevel++;
        if (subLevel >= subMenuSizes[currentMenu]) subLevel = 0;
        DisplayHelper::blankAllDisplays();
        LampsHelper::hideAllLamps();
    } else {
        currentMenu++;
        if ((!subMenuSizes[MENU_GS_ADJUSTMENTS] && currentMenu >= MENU_GS_ADJUSTMENTS) || (currentMenu >= MENU_NUMBER_OF_LEVELS)) currentMenu = MENU_STD_TESTS;
    }
}
void OperatorMenu::handleEnterSwitch() {
    // Log::printMessage(Log::LOG_OPERATOR, "Enter switch pressed");
    if (!inSubMenu) {
        inSubMenu      = true;
        subMenuChanged = true;
        subLevel       = 0;
    }
}
void OperatorMenu::handleBackSwitch() {
    // Log::printMessage(Log::LOG_OPERATOR, "Back switch pressed");
    if (inSubMenu) {
        inSubMenu = false;
        subLevel  = 0;
    } else exitRequested = true;
    DisplayHelper::blankAllDisplays();
    LampsHelper::hideAllLamps();
}

void OperatorMenu::displayMenuLevels() const {
    RPU_SetDisplayCredits(currentMenu, true, false);
    RPU_SetDisplayBallInPlay(subLevel, inSubMenu, false);
}

void OperatorMenu::run(const bool gameStateChanged) {
    if (gameStateChanged) {
        Log::printMessage(Log::LOG_OPERATOR, "Entering Operator Menu");
        currentMenu   = MENU_STD_TESTS;
        subLevel      = 0;
        inSubMenu     = false;
        exitRequested = false;
        LampsHelper::hideAllLamps();
        for (uint8_t i = 0; i < RPU_OS_NUMBER_OF_DISPLAYS; i++) {
            RPU_SetDisplayBlank(i, 0);
        }
    }

    switch (currentMenu) {
    case MENU_STD_TESTS:
        SelfTests::run();
        break;
    case MENU_STD_AUDITS:
        Audits::run();
        break;
    case MENU_STD_ADJUSTMENTS:
    case MENU_GS_ADJUSTMENTS:
        Adjustments::run();
        break;
    default:
        break;
    }

    if (subMenuChanged) {
        subMenuChanged = false;
        Log::printMessage(Log::LOG_OPERATOR, "Sub menu changed");
    }
}
void OperatorMenu::handleSwitchHit(const uint8_t switchHit) {
    if (switchHit == navigationSwitch && Time::getCurrentTime() - Time::getMostRecentSwitchHitTime() < 250) return;

    switch (currentMenu) {
    case MENU_STD_TESTS:
        SelfTests::handleSwitchHit(switchHit);
        break;
    case MENU_STD_AUDITS:
        Audits::handleSwitchHit(switchHit);
        break;
    case MENU_STD_ADJUSTMENTS:
    case MENU_GS_ADJUSTMENTS:
        Adjustments::handleSwitchHit(switchHit);
        break;
    default:
        break;
    }

    if (switchHit == SWITCH_STACK_EMPTY) return;
    if (switchHit == navigationSwitch) handleNavigationSwitch();
    if (switchHit == enterSwitch) handleEnterSwitch();
    if (switchHit == backSwitch) handleBackSwitch();
    displayMenuLevels();
}

void loadAdjItemFromEEPROM(const AdjItem* item) {
    switch (item->type) {
    case TYPE_BOOL:
        *static_cast<bool*>(item->value) = RPU_ReadByteFromEEProm(item->eepromAddress);
        break;

    case TYPE_BYTE:
        *static_cast<uint8_t*>(item->value) = RPU_ReadByteFromEEProm(item->eepromAddress);
        break;

    case TYPE_LONG: {
        *static_cast<uint32_t*>(item->value) = RPU_ReadULFromEEProm(item->eepromAddress, 0);
        break;
    }
    }
}
void loadAdjItemDefaults(const AdjItem* item) {
    switch (item->type) {
    case TYPE_BOOL:
        *static_cast<bool*>(item->value) = item->defaultValue;
        break;

    case TYPE_BYTE:
        *static_cast<uint8_t*>(item->value) = item->defaultValue;
        break;

    case TYPE_LONG: {
        *static_cast<uint32_t*>(item->value) = item->defaultValue;
        break;
    }
    }
}
void writeValueToEEPROM(const AdjItem* item, uint32_t value) {
    switch (item->type) {

    case TYPE_BOOL:
        RPU_WriteByteToEEProm(item->eepromAddress, value ? 1 : 0);
        break;

    case TYPE_BYTE:
        // Clamp value to 0–255 just in case
        if (value > 255) value = 255;
        RPU_WriteByteToEEProm(item->eepromAddress, (uint8_t)value);
        break;

    case TYPE_LONG:
        // Write 32-bit (your RPU function already handles splitting to bytes)
        RPU_WriteULToEEProm(item->eepromAddress, value);
        break;

    default:
        // if someone misconfigured the table, ignore it
        break;
    }
}

void OperatorMenu::loadDefaultFromFlash() const {
    Log::printMessage(Log::LOG_OPERATOR, "PARAMETERS -- Load all defaults from FLASH");
    for (uint8_t i = 0; i < subMenuSizes[MENU_STD_ADJUSTMENTS]; i++) {
        loadAdjItemDefaults(&standardAdjustTable[i]);
        delay(5);
    }
    for (uint8_t i = 0; i < subMenuSizes[MENU_GS_ADJUSTMENTS]; i++) {
        loadAdjItemDefaults(&specificAdjustTable[i]);
        delay(5);
    }
}
void OperatorMenu::readAllEEPROM_Values() const {
    Serial.println("PARAMETERS -- Read all parameters from EEPROM");
    for (uint8_t i = 0; i < subMenuSizes[MENU_STD_ADJUSTMENTS]; i++) {
        loadAdjItemFromEEPROM(&standardAdjustTable[i]);
        delay(5);
    }
    for (uint8_t i = 0; i < subMenuSizes[MENU_GS_ADJUSTMENTS]; i++) {
        loadAdjItemFromEEPROM(&specificAdjustTable[i]);
        delay(5);
    }
}
void OperatorMenu::writeAllDefaultEEPROM_Values() const {
    Serial.println("PARAMETERS -- Rewrite all parameters defaults to EEPROM");
    for (uint8_t i = 0; i < subMenuSizes[MENU_STD_ADJUSTMENTS]; i++) {
        writeValueToEEPROM(&standardAdjustTable[i], standardAdjustTable[i].defaultValue);
        delay(5);
    }
    for (uint8_t i = 0; i < subMenuSizes[MENU_GS_ADJUSTMENTS]; i++) {
        writeValueToEEPROM(&specificAdjustTable[i], specificAdjustTable[i].defaultValue);
        delay(5);
    }
}
