/*********************************************************************


  Sound Header File
  Version : 2.20


*********************************************************************/

#ifndef SOUND_H
#define SOUND_H

#include "System/Time.h"

#include <stdint.h>

enum AudioType : uint8_t {
    AUDIO_NONE = 0,
    AUDIO_KNOCKER,
    AUDIO_DASH32_NOTE,
    AUDIO_DASH32_SFX,
    AUDIO_DASH51,
    AUDIO_SQUAWK_TALK,
    AUDIO_WTRIG_SFX,
    AUDIO_WTRIG_VOICE,
    AUDIO_WTRIG_MUSIC,
    AUDIO_SB300_ANALOG,
    AUDIO_SB300_SQUARE,
    AUDIO_SB100,
    AUDIO_SYS7,
    AUDIO_SYS11_CPU,
    AUDIO_SYS11_FM,
    AUDIO_SOLENOIDS
};

namespace SoundHelper {
void playSoundEffect(uint16_t soundIndex, AudioType audioType);
void pushToSoundQueue(uint16_t soundIndex, Time::time_t requestedPlayTime, Time::time_t duration, AudioType audioType, uint8_t priority);
void updateSoundQueue();
void clearSoundQueue();
} // namespace SoundHelper
#endif // SOUND_H
