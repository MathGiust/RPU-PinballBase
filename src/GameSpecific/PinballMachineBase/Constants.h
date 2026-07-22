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

#define BALLS_PER_GAME 3
#define COINS_PER_CREDIT 2
#define DEFAULT_HIGH_SCORE 10000
#define MAXIMUM_NUMBER_OF_BALLS_IN_PLAY 5
#define MAXIMUM_NUMBER_OF_COINS_PER_CREDIT 4
#define MAXIMUM_NUMBER_OF_CREDITS 99
#define MAXIMUM_NUMBER_OF_TILT_WARNINGS 2
#define MULTIBALL_SAVE_NUMBER_OF_SECONDS 15
#define SOLENOID_BUFFER_TIME 250

#define BALL_THROUGH_SIZE 1
#define BALL_LOCK_SIZE 0

#define DEBOUNCE_BASE 0xF000
#define DEBOUNCED_LOW (DEBOUNCE_BASE)
#define DEBOUNCED_HIGH 0xFFFF
#define DROP_TARGET_RESET_TIMESTAMP 150
#define DROP_TARGET_RESET_DURATION 50

#define LAMP_FLASH_MIN_PERIOD 30

#define TILT_WARNING_DEBOUNCE_TIME 1000
#define INIT_NEW_BALL_DURATION 450
#define RESTART_GAME_DURATION 2000
#define LAST_BALL_HIGH_SCORE_DURATION 2000

#define NUMBER_OF_OBJECTIVES 5 // Number of Objective, each objective has an associated GameMode (1-6)

#define EXTRA_SCORING_THOUSANDS 10
#define MAX_PLAYFIELD_MULTIPLIER 1
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
