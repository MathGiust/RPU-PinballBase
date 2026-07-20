#ifndef GS_WAVETRIGGER_H
#define GS_WAVETRIGGER_H

#include "System/Sound/MusicPlaylist.h"

static Time::time_t UnstructuredPlayDurations[0] = {};

#define WTRIG_NUMBER_OF_PLAYLISTS 1
static MusicPlaylist unstructuredPlayPlaylist(0, 501, UnstructuredPlayDurations);

MusicPlaylist* musicPlaylists[WTRIG_NUMBER_OF_PLAYLISTS] = {
        &unstructuredPlayPlaylist
};

#endif
