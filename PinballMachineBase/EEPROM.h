#ifndef RPU_EEPROM_H
#define RPU_EEPROM_H

//*************************************************************************************************************************************************************/
//
// EEPROM indices location :
//
//*************************************************************************************************************************************************************/

#define RPU_HIGHSCORE_EEPROM_START_BYTE 1
#define RPU_CREDITS_EEPROM_BYTE 5

#define RPU_AWARD_SCORE_1_EEPROM_START_BYTE 10
#define RPU_AWARD_SCORE_2_EEPROM_START_BYTE 14
#define RPU_AWARD_SCORE_3_EEPROM_START_BYTE 18

#define RPU_TOTAL_PLAYS_EEPROM_START_BYTE 26
#define RPU_TOTAL_REPLAYS_EEPROM_START_BYTE 30
#define RPU_TOTAL_HISCORE_BEATEN_START_BYTE 34

#define RPU_CHUTE_2_COINS_START_BYTE 38
#define RPU_CHUTE_1_COINS_START_BYTE 42
#define RPU_CHUTE_3_COINS_START_BYTE 46
#define RPU_CPC_CHUTE_1_SELECTION_BYTE 50
#define RPU_CPC_CHUTE_2_SELECTION_BYTE 51
#define RPU_CPC_CHUTE_3_SELECTION_BYTE 52

#define RPU_PLAYER_1_SCORE_START_BYTE 54
#define RPU_PLAYER_2_SCORE_START_BYTE 58
#define RPU_PLAYER_3_SCORE_START_BYTE 62
#define RPU_PLAYER_4_SCORE_START_BYTE 66

#define EEPROM_BALL_SAVE_BYTE 70
#define EEPROM_FREE_PLAY_BYTE 71
#define EEPROM_TILT_WARNING_BYTE 72
#define EEPROM_NOVELTY_BYTE 73
#define EEPROM_EXTRA_BALL_SCORE_UL 80
#define EEPROM_SPECIAL_SCORE_UL 84

#define EEPROM_RPU_OS_INIT_PROOF_UL 90

#define EEPROM_OVERALL_GAIN_BYTE 94
#define EEPROM_SFX_GAIN_BYTE 95
#define EEPROM_VOICE_GAIN_BYTE 96
#define EEPROM_MUSIC_GAIN_BYTE 97
#define EEPROM_KNOCKER_MODE_BYTE 98

// Game Specific Adjustments
#define EEPROM_ADJ_ 100

#endif // RPU_EEPROM_H
