#ifndef GS_PLAYFIELD_H
#define GS_PLAYFIELD_H

#include "Switches.h"

#include <stdint.h>

enum STB : uint8_t {
    NUMBER_OF_STANDUP_TARGET_BANKS
};

enum DTB : uint8_t {
    NUMBER_OF_DROP_TARGET_BANKS
};

enum DEB : uint8_t {
    NUMBER_OF_DEBOUNCED_SWITCHES
};

/***************************************
 *
 * DECLARE ALL SWITCHES ARRAY HERE
 *
 ***************************************/

#endif // GS_PLAYFIELD_H
