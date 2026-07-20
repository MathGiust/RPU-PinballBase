
#include "System/Sound/Sound.h"

#include "PinballMachineBase/Solenoids.h"
#include "System/Scoring.h"

void Scoring::playTensSound() {
    SoundHelper::pushToSoundQueue(0, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playHundredsSound() {
    SoundHelper::pushToSoundQueue(0, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playThousandsSound() {
    SoundHelper::pushToSoundQueue(0, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playExtraSound() {
    SoundHelper::pushToSoundQueue(0, Time::getCurrentTime(), 100, AUDIO_NONE, 1);
}
void Scoring::playFastThousandSound() {
}
void Scoring::playBonusSound() {
}
