#include "Dash32.h"

#include "GameSpecific/PinballMachineBase/Sound.h"
#include "System/RPU.h"
#include "System/Time.h"

#ifdef RPU_OS_USE_DASH32

#define DASH32_SOUND_EFFECT_SIZE 40
static Dash32::DASH32_SoundEffect currentSoundEffect(DASH32_SOUND_EFFECT_SIZE);
static Time::time_t               soundEffectStartTime = 0;

void Dash32::clearSoundEntries() {
    for (uint8_t i = 0; i < DASH32_SOUND_EFFECT_SIZE; i++) {
        currentSoundEffect.soundEffect[i].soundIndex = -1;
        currentSoundEffect.soundEffect[i].whenToPlay = 0;
    }
}
void Dash32::playSoundEffect(const uint8_t soundIndex) {
    if (soundIndex >= NUMBER_OF_SFX) return;

    soundEffectStartTime = Time::getCurrentTime();
    for (uint8_t i = 0; i < DASH32_SoundEffects[soundIndex]->soundEffectSize; i++) {
        currentSoundEffect.soundEffect[i].soundIndex = DASH32_SoundEffects[soundIndex]->soundEffect[i].soundIndex;
        currentSoundEffect.soundEffect[i].whenToPlay = DASH32_SoundEffects[soundIndex]->soundEffect[i].whenToPlay;
        currentSoundEffect.soundEffect[i].retrig = DASH32_SoundEffects[soundIndex]->soundEffect[i].retrig;
    }
}
void Dash32::update() {
    for (uint8_t i = 0; i < currentSoundEffect.soundEffectSize; i++) {
        if ((soundEffectStartTime + currentSoundEffect.soundEffect[i].whenToPlay) < Time::getCurrentTime()
            && (currentSoundEffect.soundEffect[i].soundIndex != 0xFF)) {
            RPU_PlaySoundDash32(currentSoundEffect.soundEffect[i].soundIndex, currentSoundEffect.soundEffect[i].retrig);
            currentSoundEffect.soundEffect[i].retrig = false;
            currentSoundEffect.soundEffect[i].whenToPlay = 0;
            currentSoundEffect.soundEffect[i].soundIndex = -1;
        }
    }
}

#endif
