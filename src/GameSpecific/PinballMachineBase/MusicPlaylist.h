#ifndef GS_WAVETRIGGER_H
#define GS_WAVETRIGGER_H

#include "System/Sound/MusicPlaylist.h"

static Time::time_t UnstructuredPlayDurations[17] = {
        166940,
        157427,
        186453,
        179491,
        139377,
        144534,
        177423,
        110619,
        160066,
        167787,
        145612,
        113154,
        140304,
        243507,
        188871,
        127015,
        185678
};

#define WTRIG_NUMBER_OF_PLAYLISTS 1
static MusicPlaylist unstructuredPlayPlaylist(17, 501, UnstructuredPlayDurations);

MusicPlaylist* musicPlaylists[WTRIG_NUMBER_OF_PLAYLISTS] = {
        &unstructuredPlayPlaylist
};

#endif
