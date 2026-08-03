#include "Audits.h"

#include "OperatorMenu.h"
#include "System/RPU.h"

static OperatorMenu* operatorMenu           = nullptr;
static Time::time_t  lastEnterSwitchHitTime = 0;

static uint32_t readAuditFromEEPROM(const AuditItem* item) {
    switch (item->type) {
    case TYPE_BOOL:
        return RPU_ReadByteFromEEProm(item->eepromAddress) != 0;
    case TYPE_BYTE:
        return RPU_ReadByteFromEEProm(item->eepromAddress);
    case TYPE_LONG:
        return static_cast<uint32_t>(RPU_ReadULFromEEProm(item->eepromAddress));
    default:
        return 0;
    }
}

void Audits::init(OperatorMenu* _operatorMenu) {
    operatorMenu = _operatorMenu;
}
void Audits::run() {
    if (operatorMenu->hasSubMenuChanged()) {
        operatorMenu->setDisplays(readAuditFromEEPROM(&standardAuditTable[operatorMenu->getSubMenu()]));
    }
}
void Audits::handleSwitchHit(const uint8_t switchHit) {
    if (switchHit == operatorMenu->enterSwitch) {
        if (Time::getCurrentTime() - lastEnterSwitchHitTime < DOUBLE_CLICK_TIME) {
            RPU_WriteByteToEEProm(standardAuditTable[operatorMenu->getSubMenu()].eepromAddress, 0);
            operatorMenu->setDisplays(0);
        }
        lastEnterSwitchHitTime = Time::getCurrentTime();
    }
}
