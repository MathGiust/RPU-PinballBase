#ifndef GS_PLAYER_STATE_H
#define GS_PLAYER_STATE_H

#include "System/MachineState.h"

#include <stdint.h>

class PlayerState {
public:
    score_t score{0};
    uint8_t bonusMultiplier{1};
    uint8_t bonus{0};

    uint8_t dunkShotProgress{0};
    uint8_t centerSpinnerProgress{0};
    uint8_t leftSpinnerValue{0};

    bool leftStandupTargets[5]{false};
    bool saucerGLOBE_Letters[5]{false};
    bool centerGLOBE_Letters[5]{false};

    bool centerLeftSpinnerLit{false};
    bool centerRightSpinnerLit{false};
    bool gateOpen{false};
    bool LeftOutlaneLit{false};

public:
    void resetPlayer();
    void initNewBall();

    void increaseScore(const score_t _score) { score += _score; }
    void increaseBonus(const uint8_t _bonus) { bonus += _bonus; }

    uint8_t getBonus() const { return bonus; }
    uint8_t getBonusMultiplier() const { return bonusMultiplier; }
    void    decreaseBonusMultiplier() {
        if (bonusMultiplier > 0) bonusMultiplier--;
    }

    // Set and Get
    uint8_t getDunkShotProgress() const;
    void    setDunkShotProgress(uint8_t dunk_shot_progress);
    uint8_t getCenterSpinnerProgress() const;
    void    setCenterSpinnerProgress(uint8_t center_spinner_progress);
    uint8_t getLeftSpinnerValue() const;
    void    setLeftSpinnerValue(uint8_t left_spinner_value);
    bool    isCenterLeftSpinnerLit() const;
    void    setCenterLeftSpinnerLit(bool center_left_spinner_lit);
    bool    isCenterRightSpinnerLit() const;
    void    setCenterRightSpinnerLit(bool center_right_spinner_lit);
    bool    isGateOpen() const;
    void    setGateOpen(bool gate_open);
    bool    isLeftOutlaneLit() const;
    void    setLeftOutlaneLit(bool left_outlane_lit);

    // Get and Load array
    bool* getSaucerGLOBE_Array();
    bool* getCenterGLOBE_Array();
    void  setCenterGLOBE_Letter(uint8_t letterToSet, bool value);
};

#endif // GS_PLAYER_STATE_H
