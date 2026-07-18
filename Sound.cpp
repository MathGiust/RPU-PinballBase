
#include "System/Sound/Sound.h"

#include "GameSpecific/PinballMachineBase/PinballBase.h"
#include "System/Scoring.h"

void Scoring::playTensSound() {
    SoundHelper::pushToSoundQueue(0, 0, 0, AUDIO_DASH32_SFX, 1);
}
void Scoring::playHundredsSound() {
    SoundHelper::pushToSoundQueue(0, 0, 0, AUDIO_DASH32_SFX, 1);
}
void Scoring::playThousandsSound() {
    SoundHelper::pushToSoundQueue(0, 0, 0, AUDIO_DASH32_SFX, 1);
}
void Scoring::playExtraSound() {
    SoundHelper::pushToSoundQueue(0, 0, 0, AUDIO_DASH32_SFX, 1);
}
void Scoring::playFastThousandSound() {
    SoundHelper::pushToSoundQueue(0, 0, 0, AUDIO_DASH32_SFX, 1);
}
void Scoring::playBonusSound() {
    SoundHelper::pushToSoundQueue(0, 0, 0, AUDIO_DASH32_SFX, 1);
}
