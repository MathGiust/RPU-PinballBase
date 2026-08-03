#ifndef SCORING_H
#define SCORING_H

#include "System/MachineState.h"

enum ScoringStackIDs : uint8_t {
    STACK_TENS,
    STACK_HUNDRED,
    STACK_THOUSAND,
    STACK_EXTRA,
    STACK_FAST_THOUSAND,
    STACK_BONUS,
    NUMBER_OF_STACKS
};

namespace Scoring {

// Individual stacks control
void     addToStack(ScoringStackIDs stack, uint16_t amountToAdd);
void     emptyStack(ScoringStackIDs stack);
void     setSoundState(ScoringStackIDs stack, bool state);
void     setTimeToWait(ScoringStackIDs stack, Time::time_t timeToWait);
uint16_t getStack(ScoringStackIDs stack);

// Playfield-X
uint8_t getPlayfieldX();
void    setPlayfieldX(uint8_t value);

score_t addScoreToStacks(score_t score);
void    increaseCurrentPlayerScore(score_t valueToAdd);

void updateScoring();
void sortStacks();
void emptyAllStacks();
bool getAllStacksEmpty();

// Scoring status
void setScoringState(bool state);
void setSortingState(bool state);
void setAllSoundState(bool state);
void setTensOfThousandAnimationType(bool value);

// Sound
void playTensSound();
void playHundredsSound();
void playThousandsSound();
void playExtraSound();
void playFastThousandSound();
void playBonusSound();

void initScoring(MachineState& machineStatePtr, uint8_t _extraScoringThousand, uint8_t _maxPlayfieldMultiplier);
void setPriority(const ScoringStackIDs priority[6]);
} // namespace Scoring

#endif
