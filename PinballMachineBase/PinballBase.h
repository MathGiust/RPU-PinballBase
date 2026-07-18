#ifndef RPU_STATES_H
#define RPU_STATES_H

#include "Constants.h"
#include "EEPROM.h"
#include "Lamps.h"
#include "Solenoids.h"
#include "Sound.h"
#include "Switches.h"
#include "System/MachineState.h"

//*************************************************************************************************************************************************************/
//
// Test and audit
//
//*************************************************************************************************************************************************************/

// NORMAL
#define MACHINE_STATE_INITIALIZE 0
#define MACHINE_STATE_ATTRACT 1
#define MACHINE_STATE_INIT_GAMEPLAY 2
#define MACHINE_STATE_INIT_NEW_BALL 3
#define MACHINE_STATE_NORMAL_GAMEPLAY 4
#define MACHINE_STATE_COUNTDOWN_BONUS 99
#define MACHINE_STATE_BALL_OVER 100
#define MACHINE_STATE_RESTART_GAME 101
#define MACHINE_STATE_MATCH_MODE 110

// TESTS :
#define MACHINE_STATE_TEST_LAMPS (-1)
#define MACHINE_STATE_TEST_DISPLAYS (-2)
#define MACHINE_STATE_TEST_SOLENOIDS (-3)
#define MACHINE_STATE_TEST_SWITCHES (-4)
#define MACHINE_STATE_TEST_SOUNDS (-5)

// AUDITS :
#define MACHINE_STATE_AUDIT_CREDITS (-6)
#define MACHINE_STATE_AUDIT_TOTAL_PLAYS (-7)
#define MACHINE_STATE_AUDIT_TOTAL_REPLAYS (-8)
#define MACHINE_STATE_AUDIT_HISCR_BEAT (-9)
#define MACHINE_STATE_AUDIT_CHUTE_1_COINS (-10)
#define MACHINE_STATE_AUDIT_CHUTE_2_COINS (-11)
#define MACHINE_STATE_AUDIT_CHUTE_3_COINS (-12)

// ADJUST :
#define MACHINE_STATE_ADJ_SCORE_LEVEL_1 (-13)
#define MACHINE_STATE_ADJ_SCORE_LEVEL_2 (-14)
#define MACHINE_STATE_ADJ_SCORE_LEVEL_3 (-15)
#define MACHINE_STATE_ADJ_HISCR (-16)
#define MACHINE_STATE_ADJ_FREEPLAY (-17)
#define MACHINE_STATE_ADJ_BALL_SAVE (-18)
#define MACHINE_STATE_ADJ_NUM_TILT_WARN (-19)
#define MACHINE_STATE_ADJ_NOVELTY (-20)
#define MACHINE_STATE_ADJ_EXTRA_BALL_SCORE (-21)
#define MACHINE_STATE_ADJ_SPECIAL_SCORE (-22)
#define MACHINE_STATE_ADJ_KNOCKER_MODE (-23)
#define MACHINE_STATE_ADJ_OVERALL_GAIN (-24)
#define MACHINE_STATE_ADJ_SFX_GAIN (-25)
#define MACHINE_STATE_ADJ_VOICE_GAIN (-26)
#define MACHINE_STATE_ADJ_MUSIC_GAIN (-27)

// GAME SPECIFIC
#define MACHINE_STATE_TEST_DONE (-28)

#endif // RPU_STATES_H
