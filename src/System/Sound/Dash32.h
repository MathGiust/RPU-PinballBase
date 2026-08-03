#ifndef RPU_DASH32_H
#define RPU_DASH32_H

#include "GameSpecific/RPU_Config.h"
#include "System/Time.h"

#include <stdint.h>

#ifdef RPU_OS_USE_DASH32

namespace Dash32 {
void clearSoundEntries();
void playSoundEffect(uint8_t soundIndex);
void update();

struct DASH32_soundEntry {
    uint8_t      soundIndex;
    bool         retrig;
    Time::time_t whenToPlay;
};
struct DASH32_SoundEffect {
    // Constructor for dynamic allocation (your original one)
    explicit DASH32_SoundEffect(const uint8_t size)
        : soundEffectSize(size) {
        this->soundEffect = new DASH32_soundEntry[this->soundEffectSize];
    }

    // Constructor for static const data
    DASH32_SoundEffect(const uint8_t size, DASH32_soundEntry* data)
        : soundEffectSize(size), soundEffect(data) {}
    uint8_t            soundEffectSize;
    DASH32_soundEntry* soundEffect; // Make pointer const to data
};
} // namespace Dash32

#define DASH32_SFX(name, size, ...)                                            \
    static Dash32::DASH32_soundEntry        name##_data[size] = {__VA_ARGS__}; \
    static const Dash32::DASH32_SoundEffect name = Dash32::DASH32_SoundEffect(size, name##_data);

#endif
#endif
