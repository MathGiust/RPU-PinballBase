#include "WaveTriggerHandler.h"

#include "GameSpecific/PinballMachineBase/Constants.h"
#include "GameSpecific/PinballMachineBase/MusicPlaylist.h"
#include "GameSpecific/RPU_Config.h"
#include "WaveTriggerSerial.h"

static gpstarAudio gpstar;

static uint16_t numberOfTracks = 0;
static bool     waveTriggerFound = false;

struct TrackHandler {
    int          currentTrack = 0;
    Time::time_t startTime = 0;
    Time::time_t endTime = 0;

    void startTrack(const int newTrack, const Time::time_t duration) {
        currentTrack = newTrack;
        startTime = Time::getCurrentTime();
        endTime = Time::getCurrentTime() + duration;
    }
    void stopTrack() {
        currentTrack = 0;
        startTime = 0;
        endTime = 0;
    }
};
static TrackHandler MUSIC_Handler;
static TrackHandler SFX_Handler;
static TrackHandler VOICE_Handler;

uint8_t currentPlaylist = 0;

void SoundHelper::WaveTrigger::init() {
    WTSerial.begin(57600); // When using AltSoftSerial.
    gpstar.start(WTSerial);

    /*
    delay(10);

    gpstar.hello();
    delay(500);

    if (gpstar.gpstarAudioHello()) {
        Serial.println("GPSTAR : Connected");
        numberOfTracks = gpstar.getNumTracks();
        waveTriggerFound = true;
    }
    */
    gpstar.stopAllTracks();
}
void SoundHelper::WaveTrigger::update() {
    gpstar.update();
}

void SoundHelper::WaveTrigger::playSFX(const uint16_t soundIndex, const int8_t gain) {
    SFX_Handler.startTrack(soundIndex, 0);

    if (gpstar.isTrackPlaying(soundIndex)) gpstar.trackStop(soundIndex);
    gpstar.trackPlayPoly(soundIndex);
    gpstar.trackGain(soundIndex, gain);
}
void SoundHelper::WaveTrigger::stopSFX(const uint16_t soundIndex) {
    gpstar.trackStop(soundIndex);
    SFX_Handler.stopTrack();
}

void SoundHelper::WaveTrigger::playVOICE(uint16_t soundIndex, int8_t gain) {
}
void SoundHelper::WaveTrigger::stopVOICE(uint16_t soundIndex, int8_t gain) {
}

void SoundHelper::WaveTrigger::playMUSIC(const uint16_t soundIndex, const int8_t gain) {
    if (!BACKGROUND_MUSIC_ENABLE) return;
    if (MUSIC_Handler.currentTrack == soundIndex) return;

    stopMUSIC(); // Stop the previous background song
    MUSIC_Handler.startTrack(soundIndex, 0);

    gpstar.trackPlayPoly(soundIndex, true); // Start the song on the WT
    gpstar.trackLoop(soundIndex, true);     // Enable looping
    gpstar.trackGain(soundIndex, gain);     // Sets the gain
}
void SoundHelper::WaveTrigger::stopMUSIC() {
    gpstar.trackStop(MUSIC_Handler.currentTrack);
    MUSIC_Handler.stopTrack();
}
void SoundHelper::WaveTrigger::fadeMUSIC(const int8_t gain, const int duration, const bool stopSong) {
    gpstar.trackFade(MUSIC_Handler.currentTrack, gain, duration, stopSong);
}

void SoundHelper::Playlist::start(const uint8_t newPlaylist, const uint16_t newTrack) {
    if (newPlaylist >= WTRIG_NUMBER_OF_PLAYLISTS) return;
    if (currentPlaylist == newPlaylist && musicPlaylists[currentPlaylist]->isRunning()) return;

    currentPlaylist = newPlaylist;
    musicPlaylists[currentPlaylist]->start(newTrack);
}
void SoundHelper::Playlist::stopCurrent() {
    if (currentPlaylist >= WTRIG_NUMBER_OF_PLAYLISTS) return;
    musicPlaylists[currentPlaylist]->stop();
}
void SoundHelper::Playlist::resetPlayedTracks() {
    if (currentPlaylist >= WTRIG_NUMBER_OF_PLAYLISTS) return;
    musicPlaylists[currentPlaylist]->resetPlayedTracks();
}
void SoundHelper::Playlist::update() {
    if (currentPlaylist >= WTRIG_NUMBER_OF_PLAYLISTS) return;
    musicPlaylists[currentPlaylist]->update();
}
