#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include "OperatorMenu/Audits.h"
#include "Playfield/Debounces.h"
#include "Playfield/DropTargets.h"
#include "Playfield/StandupTargets.h"
#include "System/Time.h"

typedef uint32_t score_t;

#define TILT_WARNING_DEBOUNCE_TIME 1000

enum NoveltyModes : uint8_t {
    NOVELTY_POINTS_AND_EB = 0,
    NOVELTY_EB_ONLY,
    NOVELTY_POINTS_ONLY,
};
enum KnockerModes : uint8_t {
    KNOCKER_DISABLED = 0,
    KNOCKER_SOL_ONLY,
    KNOCKER_SOUND_ONLY,
    KNOCKER_SOUND_AND_SOL,
};

class MachineState final {
protected:
    const uint8_t   maxNumberOfPlayers;
    score_t* const* scores;

    const uint8_t outholeSwitch;
    const uint8_t coinDropSwitches[3];

    bool tilted;
    bool currentScoreFlashing;

    uint8_t currentBallInPlay;
    uint8_t numberOfTiltWarningRemaining;
    uint8_t numberOfPlayers;
    uint8_t currentPlayerNumber;
    bool    playfieldValidated;

    uint8_t numberOfBallsInPlay;

    Time::time_t gameStartTime;
    Time::time_t ballStartTime;
    Time::time_t ballSaveStartTime;

    static uint8_t mostRecentSwitchHit;
    static bool    samePlayerShootsAgain;

public:
    bool ballInSaucer;

    const uint8_t          numberOfDropTargetBanks;
    DropTargetBank* const* DTB;

    const uint8_t             numberOfStandupTargetBanks;
    StandupTargetBank* const* STB;

    const uint8_t           numberOfDebouncedSwitches;
    DebouncedSwitch* const* DEB;

    // EEPROM
    static struct EEPROMData {
        static bool freeplay;

        static uint8_t credits;
        static uint8_t ballSaveDuration;
        static uint8_t numberOfTiltWarnings;

        static uint8_t noveltyMode;
        static score_t extraBallScore;
        static score_t specialScore;

        static uint8_t knockerMode;
        static uint8_t overallGain;
        static uint8_t SFX_Gain;
        static uint8_t VOICE_Gain;
        static uint8_t MUSIC_Gain;

        static score_t highScore;
        static score_t replayScores[3];
    } EEPROMData;

    explicit MachineState(
            uint8_t _maxNumberOfPlayers, score_t* const* _scores,
            uint8_t coinDrop1, uint8_t coinDrop2, uint8_t coinDrop3,
            uint8_t _numberOfDropTargetBanks, DropTargetBank* const* _dropTargetBanks,
            uint8_t _numberOfStandupTargetBanks, StandupTargetBank* const* _standupTargetBanks,
            uint8_t _numberOfDebouncedSwitches, DebouncedSwitch* const* _debouncedSwitches,
            uint8_t _outholeSwitch
    );
    ~MachineState() = default;

    // Game state functions
    void resetGame();
    void initNewGame();
    void initNewBall();

    // System switches
    void manageCoinDrop(uint8_t switchHit) const;
    void manageTiltWarning();

    // Scores
    void increaseCurrentPlayerScore(const score_t scoreToAdd) const {
        if (!tilted) *scores[currentPlayerNumber] += scoreToAdd;
        Time::updateLastScoreChangeTime();
    }
    void     setScore(const uint8_t playerNumber, const score_t _score) const { *scores[playerNumber] = _score; }
    void     setHighScore(const score_t score) { EEPROMData::highScore = score; }
    score_t  getHighScore() { return EEPROMData::highScore; }
    score_t  getPlayerScore(const uint8_t player) const { return player < maxNumberOfPlayers ? *scores[player] : 0; }
    score_t* getPlayerScorePtr(const uint8_t player) const { return player < maxNumberOfPlayers ? scores[player] : nullptr; }

    bool isEnoughCredit();
    void addPlayer();

    bool increaseCurrentPlayerNumber();

    static void awardExtraBall();
    static void awardSpecial();

    // Setters
    void setPlayfieldValidated(const bool _playFieldValidated) { playfieldValidated = _playFieldValidated; }
    void setBallSaveStartTime(const Time::time_t _ballSaveStartTime) { ballSaveStartTime = _ballSaveStartTime; }
    void setBallInPlay(const uint8_t _ballInPlay) { currentBallInPlay = _ballInPlay; }

    // Getters
    uint8_t      getCurrentPlayerNumber() const { return currentPlayerNumber; }
    uint8_t      getNumberOfPlayers() const { return numberOfPlayers; }
    uint8_t      getCredits() { return EEPROMData::credits; }
    uint8_t      getBallInPlay() const { return currentBallInPlay; }
    uint8_t      getNumberOfTiltWarningRemaining() const { return numberOfTiltWarningRemaining; }
    bool         getTilted() const { return tilted; }
    bool         getPlayfieldValidated() const { return playfieldValidated; }
    bool         isCurrentPlayerNumberEven() const { return currentPlayerNumber % 2; }
    bool         getSamePlayerShootsAgain() { return samePlayerShootsAgain; }
    Time::time_t getBallSaveStartTime() const { return ballSaveStartTime; }
    Time::time_t getBallSaveDuration() { return EEPROMData::ballSaveDuration * 1000; }

    // Static
    static void    setMostRecentSwitchHit(const uint8_t switchHit) { mostRecentSwitchHit = switchHit; }
    static uint8_t getMostRecentSwitchHit() { return mostRecentSwitchHit; }
    static void    setNumberOfCreditsFromEEPROM() { EEPROMData::credits = RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE); }
    static void    setSamePlayerShootsAgain(const bool _samePlayerShootsAgain) { samePlayerShootsAgain = _samePlayerShootsAgain; }

    static uint8_t getOverallGain() { return EEPROMData::overallGain; }
    static uint8_t getSFX_Gain() { return EEPROMData::SFX_Gain; }
    static uint8_t getVOICE_Gain() { return EEPROMData::VOICE_Gain; }
    static uint8_t getMUSIC_Gain() { return EEPROMData::MUSIC_Gain; };
};

#endif
