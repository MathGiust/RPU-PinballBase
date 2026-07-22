#ifndef RPU_CONSTANTS_H
#define RPU_CONSTANTS_H

#include <stdint.h>

#define DEV_MODE 1
#define DEBUG_MESSAGES 1

//*************************************************************************************************************************************************************/
//
// Game Constants
//
//*************************************************************************************************************************************************************/

#define DROP_TARGET_RESET_TIMESTAMP 150
#define DROP_TARGET_RESET_DURATION 50

#define LAMP_FLASH_MIN_PERIOD 30

#define INIT_NEW_BALL_DURATION 900
#define RESTART_GAME_DURATION 200
#define LAST_BALL_HIGH_SCORE_DURATION 2000

#define MAX_BONUS_MULTIPLIER 5
#define MAX_BONUS 49

#define TIMED_SOUND_STACK_SIZE 20
#define WTRIG_BASE_GAIN (-3)
#define WAV_TRIGGER_TIMESTAMP 10
#define BACKGROUND_MUSIC_ENABLE 1

//*************************************************************************************************************************************************************/
//
// Modes and Objectives
//
//*************************************************************************************************************************************************************/

enum GameModeIDs : uint8_t {
    GAMEMODE_UNSTRUCTURED_PLAY = 0,
    GAMEMODE_GREEN,
    GAMEMODE_YELLOW,
    GAMEMODE_RED,
    GAMEMODE_COMET,
    GAMEMODE_BLACKOUT,
    GAMEMODE_MAIN_WIZARD,
    GAMEMODE_SKILL_SHOT,
    NUMBER_OF_MODES
};

#endif // RPU_CONSTANTS_H
