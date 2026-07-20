
#include "System/Sound/Sound.h"

#include "PinballMachineBase/Solenoids.h"
#include "System/Scoring.h"

void Scoring::playTensSound() {
    SoundHelper::pushToSoundQueue(SOL_CHIME_10, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playHundredsSound() {
    SoundHelper::pushToSoundQueue(SOL_CHIME_100, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playThousandsSound() {
    SoundHelper::pushToSoundQueue(SOL_CHIME_1000, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playExtraSound() {
    SoundHelper::pushToSoundQueue(SOL_CHIME_10000, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playFastThousandSound() {
}
void Scoring::playBonusSound() {
}
