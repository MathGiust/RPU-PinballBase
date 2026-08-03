#include "Scoring.h"

#include "Time.h"

static MachineState* machineState             = nullptr;
static uint8_t       EXTRA_SCORING_THOUSANDS  = 10;
static uint8_t       MAX_PLAYFIELD_MULTIPLIER = 1;

// Scoring status
static bool scoringEnabled   = false;
static bool sortStackEnabled = false;

// Scoring parameters
static bool    extraScoringType = false;
static uint8_t playfieldX       = 1;

static ScoringStackIDs stackPriority[NUMBER_OF_STACKS] = {
        STACK_FAST_THOUSAND,
        STACK_EXTRA,
        STACK_TENS,
        STACK_HUNDRED,
        STACK_THOUSAND,
        STACK_BONUS
};
static constexpr score_t stackScore[NUMBER_OF_STACKS] = {
        10,   // TENS
        100,  // HUNDREDS
        1000, // THOUSANDS
        0,    // TENS OF THOUSANDS
        1000, // FAST THOUSANDS
        0,    // BONUS
};

class ScoringStack {
public:
    ScoringStackIDs stackID;
    uint16_t        stack;
    Time::time_t    timeToWait;
    Time::time_t    lastScoringTime;
    uint8_t         priority;
    bool            soundState;
    void (*playSound)();

    explicit ScoringStack(const ScoringStackIDs ID, void (*soundFunction)())
        : stackID(ID),
          stack(0),
          timeToWait(0),
          lastScoringTime(0),
          priority(0),
          soundState(false),
          playSound(soundFunction) {}
    virtual ~ScoringStack() = default;

    bool isStackUpdateAvailable() const {
        if (this->stack
            && Time::getCurrentTime() - this->lastScoringTime > this->timeToWait
            && Time::getCurrentTime() - Time::getLastScoringTime() > this->timeToWait) return true;
        return false;
    }
    bool isWaitTimeOver() const {
        return Time::getCurrentTime() - lastScoringTime > this->timeToWait;
    }
    virtual void awardStack() {
        Scoring::increaseCurrentPlayerScore(stackScore[stackID]);
    }
};
class ExtraStack : public ScoringStack {
public:
    ExtraStack(const ScoringStackIDs ID, void (*soundFunction)()) : ScoringStack(ID, soundFunction) {};

    void awardStack() override {
        Scoring::addToStack(STACK_FAST_THOUSAND, EXTRA_SCORING_THOUSANDS);
    }
};
class BonusStack : public ScoringStack {
public:
    BonusStack(const ScoringStackIDs ID, void (*soundFunction)()) : ScoringStack(ID, soundFunction) {};

    void awardStack() override {
        // machineState->getCurrentPlayer()->increaseBonus(1);
    }
};

static ScoringStack tensStack(STACK_TENS, Scoring::playTensSound);
static ScoringStack hundredsStack(STACK_HUNDRED, Scoring::playHundredsSound);
static ScoringStack thousandsStack(STACK_THOUSAND, Scoring::playThousandsSound);
static ScoringStack fastThousandStack(STACK_FAST_THOUSAND, Scoring::playFastThousandSound);
static ExtraStack   extraStack(STACK_EXTRA, Scoring::playExtraSound);
static BonusStack   bonusStack(STACK_BONUS, Scoring::playBonusSound);

static ScoringStack* scoringStacks[NUMBER_OF_STACKS] = {
        &tensStack,
        &hundredsStack,
        &thousandsStack,
        &extraStack,
        &fastThousandStack,
        &bonusStack
};

// Individual stacks control
void Scoring::addToStack(const ScoringStackIDs stack, const uint16_t amountToAdd) {
    scoringStacks[stack]->stack += amountToAdd;
    if (sortStackEnabled) sortStacks();
}
void Scoring::emptyStack(const ScoringStackIDs stack) {
    scoringStacks[stack]->stack = 0;
}
void Scoring::setTimeToWait(const ScoringStackIDs stack, const Time::time_t timeToWait) {
    scoringStacks[stack]->timeToWait = timeToWait;
}
void Scoring::setSoundState(const ScoringStackIDs stack, const bool state) {
    scoringStacks[stack]->soundState = state;
}
uint16_t Scoring::getStack(const ScoringStackIDs stack) {
    return scoringStacks[stack]->stack;
}

void Scoring::updateScoring() {
    if (!scoringEnabled) return;

    for (uint8_t i = 0; i < NUMBER_OF_STACKS; i++) {
        ScoringStack* currentStack = scoringStacks[stackPriority[i]];
        if (!currentStack->isWaitTimeOver()) return;
        if (currentStack->stack) {
            currentStack->lastScoringTime = Time::getCurrentTime();
            currentStack->stack--;
            currentStack->awardStack();
            if (currentStack->soundState) currentStack->playSound();
            return;
        }
    }
}
void Scoring::sortStacks() {
    if (hundredsStack.stack >= 10) {
        thousandsStack.stack += hundredsStack.stack / 10;
        hundredsStack.stack %= 10;
    }
    if (thousandsStack.stack >= 10) {
        extraStack.stack += thousandsStack.stack / 10;
        thousandsStack.stack %= 10;
    }
}
void Scoring::emptyAllStacks() {
    for (auto& stack : scoringStacks) {
        stack->stack = 0;
    }
}
bool Scoring::getAllStacksEmpty() {
    for (auto& stack : scoringStacks) {
        if (stack->stack) return false;
    }
    return true;
}

uint8_t Scoring::getPlayfieldX() {
    return playfieldX;
}
void Scoring::setPlayfieldX(uint8_t value) {
    playfieldX = value;
    if (playfieldX > MAX_PLAYFIELD_MULTIPLIER) playfieldX = MAX_PLAYFIELD_MULTIPLIER;
}

void Scoring::setScoringState(const bool state) {
    scoringEnabled = state;
}
void Scoring::setSortingState(const bool state) {
    sortStackEnabled = state;
}
void Scoring::setAllSoundState(const bool state) {
    for (auto& stack : scoringStacks) {
        stack->soundState = state;
    }
}
void Scoring::setTensOfThousandAnimationType(const bool value) {
    extraScoringType = value;
}

score_t Scoring::addScoreToStacks(score_t score) {
    addToStack(STACK_EXTRA, score / (EXTRA_SCORING_THOUSANDS * 1000));
    score %= (EXTRA_SCORING_THOUSANDS * 1000);
    addToStack(STACK_THOUSAND, score / 1000);
    score %= 1000;
    addToStack(STACK_HUNDRED, score / 100);
    score %= 100;
    addToStack(STACK_TENS, score / 10);
    score %= 10;
    return score;
}
void Scoring::increaseCurrentPlayerScore(const score_t valueToAdd) {
    machineState->increaseCurrentPlayerScore(valueToAdd * playfieldX);
    Time::updateLastScoringTime();
    Time::updateLastScoreChangeTime();
}

void Scoring::initScoring(MachineState& machineStatePtr, uint8_t _extraScoringThousand, uint8_t _maxPlayfieldMultiplier) {
    machineState = &machineStatePtr;

    EXTRA_SCORING_THOUSANDS  = _extraScoringThousand;
    MAX_PLAYFIELD_MULTIPLIER = _maxPlayfieldMultiplier;

    for (auto& stack : scoringStacks) {
        stack->stack           = 0;
        stack->lastScoringTime = 0;
        stack->soundState      = false;
        stack->timeToWait      = 100;
        stack->priority        = 0;
    }
    scoringStacks[STACK_FAST_THOUSAND]->timeToWait = 6;
    scoringStacks[STACK_EXTRA]->timeToWait         = 250;
}
void Scoring::setPriority(const ScoringStackIDs priority[6]) {
    for (uint8_t i = 0; i < NUMBER_OF_STACKS; i++) {
        stackPriority[i] = priority[i];
    }
}
