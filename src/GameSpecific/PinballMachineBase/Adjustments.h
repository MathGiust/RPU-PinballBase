#ifndef GS_ADJUSTMENTS_H
#define GS_ADJUSTMENTS_H

#include "System/OperatorMenu/OperatorMenu.h"

#define EEPROM_ADJ_GRID_HELD_BYTE 100
#define EEPROM_ADJ_HOLD_STARS_BYTE 101
#define EEPROM_ADJ_SPINNER_DIFFICULTY_BYTE 103
#define EEPROM_ADJ_SOUND_TYPE_BYTE 102

#define EEPROM_ADJ_CAPTIVE_BALL_TIMER_BYTE 103
#define EEPROM_ADJ_BUMPER_COUNT_BYTE 104
#define EEPROM_ADJ_SPINNER_COUNT_BYTE 105

enum SoundSettingIDs : uint8_t {
    SOUND_NOISE_WITH_BACKGROUND = 0,
    SOUND_NOISE_WITHOUT_BACKGROUND,
    SOUND_SCORING_CHIME,
    SOUND_SCORING_NOISE,

};

struct GS_EEPROM {
    static uint8_t soundType;
    static bool    spinnerDifficulty;
    static bool    holdGrid;
    static bool    holdStars;

    static uint8_t captiveBallTimer;
    static uint8_t bumperCount;
    static uint8_t spinnerCount;
};

#define NUMBER_OF_GS_ADJ 7
const AdjItem specificAdjustmentTable[NUMBER_OF_GS_ADJ] = {
        {EEPROM_ADJ_GRID_HELD_BYTE,          TYPE_BOOL, ADJ_TOGGLE,  0,  1,   1, nullptr, 0, &GS_EEPROM::holdGrid         },
        {EEPROM_ADJ_HOLD_STARS_BYTE,         TYPE_BOOL, ADJ_TOGGLE,  0,  1,   1, nullptr, 0, &GS_EEPROM::holdStars        },
        {EEPROM_ADJ_SPINNER_DIFFICULTY_BYTE, TYPE_BOOL, ADJ_TOGGLE,  0,  1,   1, nullptr, 0, &GS_EEPROM::spinnerDifficulty},
        {EEPROM_ADJ_SOUND_TYPE_BYTE,         TYPE_BYTE, ADJ_RANGE,   0,  3,   1, nullptr, 1, &GS_EEPROM::soundType        },
        {EEPROM_ADJ_CAPTIVE_BALL_TIMER_BYTE, TYPE_BYTE, ADJ_RANGE,   0,  20,  1, nullptr, 1, &GS_EEPROM::captiveBallTimer },
        {EEPROM_ADJ_BUMPER_COUNT_BYTE,       TYPE_BYTE, ADJ_STEPPED, 10, 100, 5, nullptr, 1, &GS_EEPROM::bumperCount      },
        {EEPROM_ADJ_SPINNER_COUNT_BYTE,      TYPE_BYTE, ADJ_STEPPED, 10, 100, 5, nullptr, 1, &GS_EEPROM::spinnerCount     },
};

#endif // GS_ADJUSTMENTS_H
