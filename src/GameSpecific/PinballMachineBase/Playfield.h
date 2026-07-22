#ifndef GS_PLAYFIELD_H
#define GS_PLAYFIELD_H

#include "Solenoids.h"
#include "Switches.h"

#include <stdint.h>

enum DTB : uint8_t {
    DTB_LEFT = 0,
    DTB_MID,
    DTB_RIGHT,
    NUMBER_OF_DROP_TARGET_BANKS
};

enum STB : uint8_t {
    NUMBER_OF_STANDUP_TARGET_BANKS
};

enum DEB : uint8_t {
    NUMBER_OF_DEBOUNCED_SWITCHES
};

static uint8_t DTB_LEFT_SWITCHES[] = {
        SW_LEFT_DROP_TARGET_1,
        SW_LEFT_DROP_TARGET_2,
        SW_LEFT_DROP_TARGET_3
};
static uint8_t DTB_LEFT_SOLS[] = {
        SOL_LEFT_DROP_TARGET_RESET
};
static bool DTB_LEFT_STATUS[3] = {};

static uint8_t DTB_MID_SWITCHES[] = {
        SW_CENTER_DROP_TARGET_1,
        SW_CENTER_DROP_TARGET_2,
        SW_CENTER_DROP_TARGET_3
};
static uint8_t DTB_MID_SOLS[] = {
        SOL_CENTER_DROP_TARGET_RESET
};
static bool DTB_MID_STATUS[3] = {};

static uint8_t DTB_RIGHT_SWITCHES[] = {
        SW_RIGHT_DROP_TARGET_1,
        SW_RIGHT_DROP_TARGET_2,
        SW_RIGHT_DROP_TARGET_3
};
static uint8_t DTB_RIGHT_SOLS[] = {
        SOL_RIGHT_DROP_TARGET_RESET
};
static bool DTB_RIGHT_STATUS[3] = {};

#endif // GS_PLAYFIELD_H
