#ifndef RPU_ADJUSTMENTS_H
#define RPU_ADJUSTMENTS_H

#include "OperatorMenu.h"
#include "System/MachineState.h"

#include <stdint.h>
#define RPU_AWARD_SCORE_1_EEPROM_UL 10
#define RPU_AWARD_SCORE_2_EEPROM_UL 14
#define RPU_AWARD_SCORE_3_EEPROM_UL 18
#define RPU_HIGHSCORE_EEPROM_UL 1
#define RPU_PLAYER_1_SCORE_UL 54
#define RPU_PLAYER_2_SCORE_UL 58
#define RPU_PLAYER_3_SCORE_UL 62
#define RPU_PLAYER_4_SCORE_UL 66
#define EEPROM_BALL_SAVE_BYTE 70
#define EEPROM_FREE_PLAY_BYTE 71
#define EEPROM_TILT_WARNING_BYTE 72
#define EEPROM_NOVELTY_MODE_BYTE 73
#define EEPROM_EXTRA_BALL_SCORE_UL 80
#define EEPROM_SPECIAL_SCORE_UL 84
#define EEPROM_OVERALL_GAIN_BYTE 94
#define EEPROM_SFX_GAIN_BYTE 95
#define EEPROM_VOICE_GAIN_BYTE 96
#define EEPROM_MUSIC_GAIN_BYTE 97
#define EEPROM_KNOCKER_MODE_BYTE 98

#define NUMBER_OF_STD_ADJ 15

static const AdjItem standardAdjustmentTable[NUMBER_OF_STD_ADJ] = {
        {RPU_AWARD_SCORE_1_EEPROM_UL, TYPE_LONG, ADJ_STEPPED, 0, 9999990,    10000, nullptr, 200000, &MachineState::EEPROMData::replayScores[0]     },
        {RPU_AWARD_SCORE_2_EEPROM_UL, TYPE_LONG, ADJ_STEPPED, 0, 9999990,    10000, nullptr, 300000, &MachineState::EEPROMData::replayScores[1]     },
        {RPU_AWARD_SCORE_3_EEPROM_UL, TYPE_LONG, ADJ_STEPPED, 0, 9999990,    10000, nullptr, 400000, &MachineState::EEPROMData::replayScores[2]     },
        {RPU_HIGHSCORE_EEPROM_UL,     TYPE_LONG, ADJ_STEPPED, 0, 0xFFFFFFFF, 10000, nullptr, 500000, &MachineState::EEPROMData::highScore           },
        {EEPROM_FREE_PLAY_BYTE,       TYPE_BOOL, ADJ_TOGGLE,  0, 1,          1,     nullptr, 1,      &MachineState::EEPROMData::freeplay            },
        {EEPROM_BALL_SAVE_BYTE,       TYPE_BYTE, ADJ_RANGE,   0, 20,         1,     nullptr, 0,      &MachineState::EEPROMData::ballSaveDuration    },
        {EEPROM_TILT_WARNING_BYTE,    TYPE_BYTE, ADJ_RANGE,   0, 2,          1,     nullptr, 0,      &MachineState::EEPROMData::numberOfTiltWarnings},
        {EEPROM_OVERALL_GAIN_BYTE,    TYPE_BYTE, ADJ_RANGE,   0, 20,         1,     nullptr, 1,      &MachineState::EEPROMData::overallGain         },
        {EEPROM_SFX_GAIN_BYTE,        TYPE_BYTE, ADJ_RANGE,   0, 70,         1,     nullptr, 1,      &MachineState::EEPROMData::SFX_Gain            },
        {EEPROM_VOICE_GAIN_BYTE,      TYPE_BYTE, ADJ_RANGE,   0, 70,         1,     nullptr, 1,      &MachineState::EEPROMData::VOICE_Gain          },
        {EEPROM_MUSIC_GAIN_BYTE,      TYPE_BYTE, ADJ_RANGE,   0, 70,         1,     nullptr, 1,      &MachineState::EEPROMData::MUSIC_Gain          },
        {EEPROM_NOVELTY_MODE_BYTE,    TYPE_BYTE, ADJ_RANGE,   0, 2,          1,     nullptr, 0,      &MachineState::EEPROMData::noveltyMode         },
        {EEPROM_EXTRA_BALL_SCORE_UL,  TYPE_LONG, ADJ_STEPPED, 0, 9999990,    10000, nullptr, 25000,  &MachineState::EEPROMData::extraBallScore      },
        {EEPROM_SPECIAL_SCORE_UL,     TYPE_LONG, ADJ_STEPPED, 0, 9999990,    10000, nullptr, 50000,  &MachineState::EEPROMData::specialScore        },
        {EEPROM_KNOCKER_MODE_BYTE,    TYPE_BYTE, ADJ_RANGE,   0, 3,          1,     nullptr, 3,      &MachineState::EEPROMData::knockerMode         },
};

namespace Adjustments {
void init(OperatorMenu* _operatorMenu);
void run();
void handleSwitchHit(uint8_t switchHit);
} // namespace Adjustments

#endif // RPU_ADJUSTMENTS_H
