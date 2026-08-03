#include "LampAnimation.h"

#include <avr/pgmspace.h>

// Raw frame data, stays in flash
static constexpr uint8_t animationData0[3][RPU_NUM_LAMP_BANKS] PROGMEM = {
        {0b00000001, 0x00, 0x00, 0x00},
        {0b00000011, 0x00, 0x00, 0x00},
        {0b00000111, 0x00, 0x00, 0x00},
};

static constexpr uint8_t animationData1[4][RPU_NUM_LAMP_BANKS] PROGMEM = {
        {0xFF, 0x00, 0x00, 0x00},
        {0x00, 0xFF, 0x00, 0x00},
        {0x00, 0x00, 0xFF, 0x00},
        {0x00, 0x00, 0x00, 0xFF},
};

// One const instance per animation
LampAnimation anim0(animationData0, 3);
LampAnimation anim1(animationData1, 4);

// Registry of all animations
LampAnimation* animationList[] = {
        &anim0,
        &anim1
};

uint8_t NUM_ANIMATIONS = sizeof(animationList) / sizeof(animationList[0]);
