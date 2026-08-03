#ifndef RPU_MUSIC_PLAYLIST_H
#define RPU_MUSIC_PLAYLIST_H

#include "System/Time.h"

#include <stdint.h>

class MusicPlaylist {
    bool          running;
    uint16_t      numberOfTracks;
    uint16_t      playlistStartIndex;
    uint16_t      currentTrack;
    Time::time_t  currentTrackEndTime;
    Time::time_t  currentTrackStartTime;
    Time::time_t* trackDurations;
    bool*         playedTracks;

    void setNewTrack(uint16_t newTrack);
    void playCurrentTrack() const;
    bool checkIfAllTracksPlayed() const;

    uint16_t randomizeCurrentTrack() const;

public:
    MusicPlaylist(uint16_t numberOfTracks, uint16_t playlistStartIndex, Time::time_t* trackDurations);
    void start(uint16_t trackIndex);
    void stop();
    void update();
    void resetPlayedTracks() const;
    bool isRunning() const;
};
#endif // RPU_MUSIC_PLAYLIST_H
