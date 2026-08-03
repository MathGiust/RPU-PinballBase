#ifndef RPU_LAMP_H
#define RPU_LAMP_H

#include "LampEffect.h"

#include <stdint.h>

struct Lamp {
    bool     state = false;
    uint16_t flashPeriod = 0;
    uint8_t  dim = 0;

    LampEffect lampEffect{};
};

#endif // RPU_LAMP_H
