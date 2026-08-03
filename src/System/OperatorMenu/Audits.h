#ifndef RPU_AUDITS_H
#define RPU_AUDITS_H

#include "OperatorMenu.h"

#include <stdint.h>

#define NUMBER_OF_STD_AUDITS 7

#define RPU_CREDITS_EEPROM_BYTE 5
#define RPU_TOTAL_PLAYS_EEPROM_START_BYTE 26
#define RPU_TOTAL_REPLAYS_EEPROM_START_BYTE 30
#define RPU_TOTAL_HISCORE_BEATEN_START_BYTE 34
#define RPU_CHUTE_2_COINS_START_BYTE 38
#define RPU_CHUTE_1_COINS_START_BYTE 42
#define RPU_CHUTE_3_COINS_START_BYTE 46

static constexpr AuditItem standardAuditTable[NUMBER_OF_STD_AUDITS] = {
        {RPU_CREDITS_EEPROM_BYTE,             TYPE_BYTE},
        {RPU_TOTAL_PLAYS_EEPROM_START_BYTE,   TYPE_LONG},
        {RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TYPE_LONG},
        {RPU_TOTAL_HISCORE_BEATEN_START_BYTE, TYPE_LONG},
        {RPU_CHUTE_1_COINS_START_BYTE,        TYPE_LONG},
        {RPU_CHUTE_2_COINS_START_BYTE,        TYPE_LONG},
        {RPU_CHUTE_3_COINS_START_BYTE,        TYPE_LONG},
};

namespace Audits {
void init(OperatorMenu* _operatorMenu);
void run();
void handleSwitchHit(uint8_t switchHit);
} // namespace Audits

#endif // RPU_AUDITS_H
