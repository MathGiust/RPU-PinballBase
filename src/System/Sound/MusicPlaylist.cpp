#include "MusicPlaylist.h"

#include "Sound.h"
#include "System/Utilities.h"
#include "WaveTriggerHandler.h"

MusicPlaylist::MusicPlaylist(const uint16_t numberOfTracks, const uint16_t playlistStartIndex, Time::time_t* trackDurations) {
    this->numberOfTracks = numberOfTracks;
    this->playlistStartIndex = playlistStartIndex;
    this->trackDurations = trackDurations;

    running = false;
    currentTrackStartTime = 0;
    currentTrackEndTime = 0;
    currentTrack = 0;
    playedTracks = new bool[numberOfTracks];
    Utilities::resetArray(playedTracks, numberOfTracks);
}


bool MusicPlaylist::checkIfAllTracksPlayed() const {
    if (!Utilities::checkIfArrayIsTrue(playedTracks, numberOfTracks)) return false;
    return true;
}
uint16_t MusicPlaylist::randomizeCurrentTrack() const {
    if (checkIfAllTracksPlayed()) resetPlayedTracks();
    uint16_t trackNumber = 0;

    do {
        trackNumber = random(numberOfTracks);
    } while (playedTracks[trackNumber]);
    return trackNumber;
}
void MusicPlaylist::setNewTrack(const uint16_t newTrack) {
    if (running && newTrack == currentTrack) return;
    running = true;

    currentTrack = newTrack;
    playedTracks[currentTrack] = true;
    currentTrackStartTime = Time::getCurrentTime();
    currentTrackEndTime = currentTrackStartTime + trackDurations[currentTrack];
    playCurrentTrack();
}
void MusicPlaylist::playCurrentTrack() const {
#if (DEBUG_MESSAGES)
    Serial.println("\nPLAYLIST : ");
    Serial.println(currentTrack);
    Serial.println(currentTrackStartTime);
    Serial.println(currentTrackEndTime);
#endif
    SoundHelper::playSoundEffect(currentTrack + playlistStartIndex, AUDIO_WTRIG_MUSIC);
}


void MusicPlaylist::start(uint16_t trackIndex) {
    if (trackIndex >= numberOfTracks) trackIndex = randomizeCurrentTrack();
    setNewTrack(trackIndex);
}
void MusicPlaylist::stop() {
    running = false;
    SoundHelper::WaveTrigger::fadeMUSIC(-70, 5000, true);
}
void MusicPlaylist::resetPlayedTracks() const {
    Utilities::resetArray(playedTracks, numberOfTracks);
}
void MusicPlaylist::update() {
    if (!running) return;
    if (Time::getCurrentTime() < currentTrackEndTime) return;

    setNewTrack(randomizeCurrentTrack());
}
bool MusicPlaylist::isRunning() const {
    return running;
}
