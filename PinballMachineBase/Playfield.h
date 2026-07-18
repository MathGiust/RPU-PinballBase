#ifndef GS_PLAYFIELD_H
#define GS_PLAYFIELD_H

#include "Switches.h"

#include <stdint.h>

enum DTB : uint8_t {
    DTB_INLINE,
    NUMBER_OF_DROP_TARGET_BANKS
};

enum STB : uint8_t {
    STB_LEFT,
    NUMBER_OF_STANDUP_TARGET_BANKS
};

enum DEB : uint8_t {
    NUMBER_OF_DEBOUNCED_SWITCHES
};

#endif // GS_PLAYFIELD_H
