#ifndef RPU_WAVETRIGGER_HANDLER_H
#define RPU_WAVETRIGGER_HANDLER_H

#include "System/Time.h"

#include <stdint.h>

namespace SoundHelper {
namespace WaveTrigger {

void init();
void update();

void playMUSIC(uint16_t soundIndex, int8_t gain);
void stopMUSIC();
void fadeMUSIC(int8_t gain, int duration, bool stopSong);

void playSFX(uint16_t soundIndex, int8_t gain);
void stopSFX(uint16_t soundIndex);

void playVOICE(uint16_t soundIndex, int8_t gain);
void stopVOICE(uint16_t soundIndex, int8_t gain);

bool isTrackPlaying(uint16_t trackNumber);
} // namespace WaveTrigger

namespace Playlist {
void start(uint8_t newPlaylist, uint16_t newTrack);
void stopCurrent();
void resetPlayedTracks();
void update();

Time::time_t getCurrentTrackStartTime();
} // namespace Playlist
} // namespace SoundHelper
#endif
