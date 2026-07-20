//
// Created by mathias on 5/21/26.
//

#ifndef RPU_OPERATOR_MENU_H
#define RPU_OPERATOR_MENU_H

#include "PinballBase.h"

enum NoveltyModes : uint8_t {
    NOVELTY_POINTS_AND_EB = 0,
    NOVELTY_EB_ONLY,
    NOVELTY_POINTS_ONLY,
    NUMBER_OF_NOVELTY_MODES
};
enum KnockerModes : uint8_t {
    KNOCKER_DISABLED = 0,
    KNOCKER_SOL_ONLY,
    KNOCKER_SOUND_ONLY,
    KNOCKER_SOUND_AND_SOL,
    NUMBER_OF_KNOCKER_MODES
};

enum ValueType : uint8_t {
    TYPE_BYTE,
    TYPE_LONG,
    TYPE_BOOL
};
enum AuditMode : uint8_t {
    AUDIT_READONLY,
    AUDIT_RESET_ON_DOUBLE
};
enum AdjMode : uint8_t {
    ADJ_TOGGLE,
    ADJ_RANGE,
    ADJ_STEPPED
};

struct AuditItem {
    int8_t    state;      // the negative machine state
    uint16_t  eepromAddr; // where the value is stored
    ValueType type;       // BYTE, WORD, LONG
};

struct AdjItem {
    int8_t    state;
    uint16_t  eepromAddr;
    ValueType type;
    AdjMode   mode;
    uint32_t  minValue;
    uint32_t  maxValue;
    uint32_t  step;
    uint32_t (*stepFunc)(uint32_t);
    uint32_t defaultValue;
    void*    machineStateTarget;
};

#define FIRST_AUDIT_STATE (MACHINE_STATE_AUDIT_CREDITS)
#define LAST_AUDIT_STATE (MACHINE_STATE_AUDIT_CHUTE_3_COINS)

#define FIRST_ADJ_STATE (MACHINE_STATE_ADJ_SCORE_LEVEL_1)
#define LAST_ADJ_STATE (MACHINE_STATE_TEST_DONE + 1)

inline bool isAuditState(const int s) { return s <= FIRST_AUDIT_STATE && s >= LAST_AUDIT_STATE; }
inline bool isAdjState(const int s) { return s <= FIRST_ADJ_STATE && s >= LAST_ADJ_STATE; }

const AuditItem auditTable[] = {
        {MACHINE_STATE_AUDIT_CREDITS,       RPU_CREDITS_EEPROM_BYTE,             TYPE_BYTE},
        {MACHINE_STATE_AUDIT_TOTAL_PLAYS,   RPU_TOTAL_PLAYS_EEPROM_START_BYTE,   TYPE_LONG},
        {MACHINE_STATE_AUDIT_TOTAL_REPLAYS, RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TYPE_LONG},
        {MACHINE_STATE_AUDIT_HISCR_BEAT,    RPU_TOTAL_HISCORE_BEATEN_START_BYTE, TYPE_LONG},
        {MACHINE_STATE_AUDIT_CHUTE_1_COINS, RPU_CHUTE_1_COINS_START_BYTE,        TYPE_LONG},
        {MACHINE_STATE_AUDIT_CHUTE_2_COINS, RPU_CHUTE_2_COINS_START_BYTE,        TYPE_LONG},
        {MACHINE_STATE_AUDIT_CHUTE_3_COINS, RPU_CHUTE_3_COINS_START_BYTE,        TYPE_LONG},
};
const AdjItem adjTable[] = {
        {MACHINE_STATE_ADJ_SCORE_LEVEL_1,    RPU_AWARD_SCORE_1_EEPROM_START_BYTE, TYPE_LONG, ADJ_STEPPED, 0, 9999990,                     10000, nullptr, 200000, &MachineState::getEEPROMDataPtr()->replayScores[0]     },
        {MACHINE_STATE_ADJ_SCORE_LEVEL_2,    RPU_AWARD_SCORE_2_EEPROM_START_BYTE, TYPE_LONG, ADJ_STEPPED, 0, 9999990,                     10000, nullptr, 300000, &MachineState::getEEPROMDataPtr()->replayScores[1]     },
        {MACHINE_STATE_ADJ_SCORE_LEVEL_3,    RPU_AWARD_SCORE_3_EEPROM_START_BYTE, TYPE_LONG, ADJ_STEPPED, 0, 9999990,                     10000, nullptr, 400000, &MachineState::getEEPROMDataPtr()->replayScores[2]     },
        {MACHINE_STATE_ADJ_HISCR,            RPU_HIGHSCORE_EEPROM_START_BYTE,     TYPE_LONG, ADJ_STEPPED, 0, 0xFFFFFFFF,                  10000, nullptr, 0,      &MachineState::getEEPROMDataPtr()->highScore           },
        {MACHINE_STATE_ADJ_FREEPLAY,         EEPROM_FREE_PLAY_BYTE,               TYPE_BOOL, ADJ_TOGGLE,  1, 0,                           1,     nullptr, 1,      &MachineState::getEEPROMDataPtr()->freeplay            },
        {MACHINE_STATE_ADJ_BALL_SAVE,        EEPROM_BALL_SAVE_BYTE,               TYPE_BYTE, ADJ_RANGE,   0, 20000,                       1,     nullptr, 0,      &MachineState::getEEPROMDataPtr()->ballSaveDuration    },
        {MACHINE_STATE_ADJ_NUM_TILT_WARN,    EEPROM_TILT_WARNING_BYTE,            TYPE_BYTE, ADJ_RANGE,   0, 2,                           1,     nullptr, 0,      &MachineState::getEEPROMDataPtr()->numberOfTiltWarnings},
        {MACHINE_STATE_ADJ_NOVELTY,          EEPROM_NOVELTY_BYTE,                 TYPE_BYTE, ADJ_RANGE,   0, NUMBER_OF_NOVELTY_MODES - 1, 1,     nullptr, 0,      &MachineState::getEEPROMDataPtr()->noveltyMode         },
        {MACHINE_STATE_ADJ_EXTRA_BALL_SCORE, EEPROM_EXTRA_BALL_SCORE_UL,          TYPE_LONG, ADJ_STEPPED, 0, 1000000,                     1000,  nullptr, 25000,  &MachineState::getEEPROMDataPtr()->extraBallScore      },
        {MACHINE_STATE_ADJ_SPECIAL_SCORE,    EEPROM_SPECIAL_SCORE_UL,             TYPE_LONG, ADJ_STEPPED, 0, 1000000,                     1000,  nullptr, 50000,  &MachineState::getEEPROMDataPtr()->specialScore        },
        {MACHINE_STATE_ADJ_KNOCKER_MODE,     EEPROM_KNOCKER_MODE_BYTE,            TYPE_BYTE, ADJ_RANGE,   0, NUMBER_OF_KNOCKER_MODES - 1, 1,     nullptr, 3,      &MachineState::getEEPROMDataPtr()->knockerMode         },
        {MACHINE_STATE_ADJ_OVERALL_GAIN,     EEPROM_OVERALL_GAIN_BYTE,            TYPE_BYTE, ADJ_RANGE,   0, 20,                          1,     nullptr, 1,      &MachineState::getEEPROMDataPtr()->overallGain         },
        {MACHINE_STATE_ADJ_SFX_GAIN,         EEPROM_SFX_GAIN_BYTE,                TYPE_BYTE, ADJ_RANGE,   0, 70,                          1,     nullptr, 1,      &MachineState::getEEPROMDataPtr()->SFX_Gain            },
        {MACHINE_STATE_ADJ_VOICE_GAIN,       EEPROM_VOICE_GAIN_BYTE,              TYPE_BYTE, ADJ_RANGE,   0, 70,                          1,     nullptr, 1,      &MachineState::getEEPROMDataPtr()->VOICE_Gain          },
        {MACHINE_STATE_ADJ_MUSIC_GAIN,       EEPROM_MUSIC_GAIN_BYTE,              TYPE_BYTE, ADJ_RANGE,   0, 70,                          1,     nullptr, 1,      &MachineState::getEEPROMDataPtr()->MUSIC_Gain          },
        // Game specific EEPROM parameters
};

namespace SelfTestAndAudit {
void runGameSpecificTest(int currentTest, bool currentStateChanged, uint8_t switchHit);
};

#endif // RPU_OPERATOR_MENU_H
