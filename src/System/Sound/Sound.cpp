#include "Sound.h"

#include "GameSpecific/PinballMachineBase/Solenoids.h"
#include "GameSpecific/PinballMachineBase/Constants.h"
#include "System/MachineState.h"
#include "System/RPU.h"
#include "System/Time.h"
#include "WaveTriggerHandler.h"

#include <Arduino.h>

struct TimedSoundEntry {
    uint16_t     soundNumber;
    uint8_t      priority;
    AudioType    audioType;
    Time::time_t requestedPlayTime;
    Time::time_t duration;

    void resetSoundEntry() {
        duration = 0;
        priority = 0;
        audioType = AUDIO_NONE;
        soundNumber = 0;
        requestedPlayTime = 0;
    }
};
static TimedSoundEntry currentSoundFromSoundQueue;
static TimedSoundEntry soundQueue[TIMED_SOUND_STACK_SIZE];

void SoundHelper::playSoundEffect(uint16_t soundIndex, const AudioType audioType) {
    switch (audioType) {
    default:
        break;
    case AUDIO_KNOCKER:
        // MachineState::playKnockerSound();
        break;
    case AUDIO_SOLENOIDS:
        RPU_PushToSolenoidStack(soundIndex, SOL_CHIME_STRENGTH, true);
        break;
#ifdef RPU_OS_USE_DASH32
    case AUDIO_DASH32_SFX:
        Dash32::clearSoundEntries();
        Dash32::playSoundEffect(soundIndex);
        break;
    case AUDIO_DASH32_NOTE:
        RPU_PlaySoundDash32(soundIndex, true);
        break;
#endif
#ifdef RPU_OS_USE_DASH51
    case AUDIO_DASH51:
        RPU_PlaySoundDash51(soundIndex);
        break;
#endif
#ifdef RPU_OS_USE_S_AND_T
    case AUDIO_SQUAWK_TALK:
        RPU_PlaySoundSAndT(soundIndex);
        break;
#endif
#ifdef RPU_OS_USE_WAV_TRIGGER_1p3
    case AUDIO_WTRIG_MUSIC:
        if (!MachineState::EEPROMData::MUSIC_Gain) break;
        WaveTrigger::playMUSIC(soundIndex, WTRIG_BASE_GAIN - MachineState::EEPROMData::overallGain - MachineState::EEPROMData::MUSIC_Gain);
        break;
    case AUDIO_WTRIG_SFX:
        if (!MachineState::EEPROMData::SFX_Gain) break;
        WaveTrigger::playSFX(soundIndex, WTRIG_BASE_GAIN - MachineState::EEPROMData::overallGain - MachineState::EEPROMData::SFX_Gain);
        break;
    case AUDIO_WTRIG_VOICE:
        if (!MachineState::EEPROMData::VOICE_Gain) break;
        WaveTrigger::playVOICE(soundIndex, WTRIG_BASE_GAIN - MachineState::EEPROMData::overallGain - MachineState::EEPROMData::VOICE_Gain);
        break;
#endif

#ifdef RPU_OS_USE_SB100
    case AUDIO_SB100:
        RPU_PlaySB100(soundIndex);
        break;
#endif

#ifdef RPU_OS_USE_SB300
    case AUDIO_SB300_ANALOG:
        RPU_PlaySB300Analog(0, soundIndex);
        break;
    case AUDIO_SB300_SQUARE:
        RPU_PlaySB300SquareWave(0, soundIndex);
        break;
#endif

#if defined(RPU_OS_USE_WTYPE_1_SOUND) || defined(RPU_OS_USE_WTYPE_2_SOUND)
    case AUDIO_SYS7:
        RPU_PushToSoundStack(256 * soundIndex, 1);
#endif

#ifdef RPU_OS_USE_W11_SOUND
#endif
    }
}

void SoundHelper::pushToSoundQueue(const uint16_t soundIndex, Time::time_t requestedPlayTime, const Time::time_t duration, const AudioType audioType, const uint8_t priority) {
    if (requestedPlayTime == 0) requestedPlayTime = Time::getCurrentTime();
    for (auto& i : soundQueue) {
        if (i.requestedPlayTime) continue;
        i.soundNumber = soundIndex;
        i.requestedPlayTime = requestedPlayTime;
        i.duration = duration;
        i.audioType = audioType;
        i.priority = priority;
        break;
    }
}

void SoundHelper::updateSoundQueue() {
    for (auto& currentSoundEntry : soundQueue) {
        if (currentSoundEntry.requestedPlayTime && currentSoundEntry.requestedPlayTime < Time::getCurrentTime()) {
            if (currentSoundEntry.priority >= currentSoundFromSoundQueue.priority) {
                // copy the sound parameters to the current sound
                currentSoundFromSoundQueue.duration = currentSoundEntry.duration;
                currentSoundFromSoundQueue.priority = currentSoundEntry.priority;
                currentSoundFromSoundQueue.soundNumber = currentSoundEntry.soundNumber;
                currentSoundFromSoundQueue.audioType = currentSoundEntry.audioType;
                currentSoundFromSoundQueue.requestedPlayTime = currentSoundEntry.requestedPlayTime;
                // Play current sound effect
                playSoundEffect(currentSoundFromSoundQueue.soundNumber, currentSoundFromSoundQueue.audioType);
            }
            currentSoundEntry.resetSoundEntry();
        }
    }

    // Check if current sound is over
    if (currentSoundFromSoundQueue.requestedPlayTime
        && Time::getCurrentTime() >= currentSoundFromSoundQueue.requestedPlayTime + currentSoundFromSoundQueue.duration) {
        currentSoundFromSoundQueue.resetSoundEntry();
    }
#ifdef RPU_OS_USE_DASH32
    Dash32::update();
#endif
}

void SoundHelper::clearSoundQueue() {
    for (auto& soundEntry : soundQueue) {
        soundEntry.resetSoundEntry();
    }
}
