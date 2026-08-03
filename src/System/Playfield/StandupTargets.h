/*********************************************************************


Drop Targets Header File
    Version : 1.10


*********************************************************************/

#ifndef STB_H
#define STB_H

#include "System/RPU.h"

class StandupTargetBank {
    const uint8_t        numberOfTargets;
    const uint8_t* const switchArray;

public:
    StandupTargetBank(
            uint8_t        _numberOfTargets,
            const uint8_t* _switchArray
    );
    ~StandupTargetBank() = default;

    uint8_t getTargetHitIndex(uint8_t switchHit) const;
};

#endif
