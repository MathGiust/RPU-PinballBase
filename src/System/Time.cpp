/*********************************************************************


    Time Source File
    Version : 1.10


*********************************************************************/

#include "Time.h"

#include <Arduino.h>

static bool         IT_1000ms      = false;
static Time::time_t lastSeed1000ms = 0;
static bool         IT_100ms       = false;
static Time::time_t lastSeed100ms  = 0;
static bool         IT_10ms        = false;
static Time::time_t lastSeed10ms   = 0;

static Time::time_t currentTime                = 0;
static Time::time_t mostRecentSwitchHitTime    = 0;
static Time::time_t lastTiltWarningTime        = 0;
static Time::time_t ballEnteredTroughTime      = 0;
static Time::time_t lastOutholeKickTime        = 0;
static Time::time_t lastScoringTime            = 0;
static Time::time_t lastBonusPointsTime        = 0;
static Time::time_t lastTestCycleTime          = 0;
static Time::time_t lastModeTimerUpdateTime    = 0;
static Time::time_t lastSubModeTimerUpdateTime = 0;
static Time::time_t lastLampAnimUpdateTime     = 0;
static Time::time_t lastAttractScoreUpdateTime = 0;
static Time::time_t lastDisplayUpdateTime      = 0;
static Time::time_t lastScoreChangeTime        = 0;
static Time::time_t lastUpperDropsResetTime    = 0;

// Game specific

static Time::time_t lastSaucerSwitchHitTime = 0;
static Time::time_t lastSaucerKickTime      = 0;
static Time::time_t lastAB_LaneTime         = 0;

Time::time_t Time::getElapsedTime(time_t time) {
    return abs(currentTime - time);
}

//
//  Get
//

bool Time::get1000msIT() {
    return IT_1000ms;
}
bool Time::get100msIT() {
    return IT_100ms;
}
bool Time::get10msIT() {
    return IT_10ms;
}

Time::time_t Time::getCurrentTime() {
    return currentTime;
}
Time::time_t Time::getMostRecentSwitchHitTime() {
    return mostRecentSwitchHitTime;
}
Time::time_t Time::getLastTiltWarningTime() {
    return mostRecentSwitchHitTime;
}
Time::time_t Time::getBallEnteredTroughTime() {
    return ballEnteredTroughTime;
}
Time::time_t Time::getLastOutholeKickTime() {
    return lastOutholeKickTime;
}
Time::time_t Time::getLastScoringTime() {
    return lastScoringTime;
}
Time::time_t Time::getLastBonusPointsTime() {
    return lastBonusPointsTime;
}
Time::time_t Time::getLastTestCycleTime() {
    return lastTestCycleTime;
}
Time::time_t Time::getLastModeTimerUpdateTime() {
    return lastModeTimerUpdateTime;
}
Time::time_t Time::getLastSubModeTimerUpdateTime() {
    return lastSubModeTimerUpdateTime;
}
Time::time_t Time::getLastLampAnimUpdateTime() {
    return lastLampAnimUpdateTime;
}
Time::time_t Time::getLastAttractScoreUpdateTime() {
    return lastAttractScoreUpdateTime;
}
Time::time_t Time::getLastDisplayUpdateTime() {
    return lastDisplayUpdateTime;
}
Time::time_t Time::getLastScoreChangeTime() {
    return lastScoreChangeTime;
}

//
//  Update
//

void Time::updateCurrentTime() {
    currentTime                   = millis();
    const Time::time_t seed1000ms = currentTime / 1000;
    const Time::time_t seed100ms  = currentTime / 100;
    const Time::time_t seed10ms   = currentTime / 10;

    if (seed1000ms != lastSeed1000ms) {
        lastSeed1000ms = seed1000ms; // Update seed
        IT_1000ms      = true;
    } else IT_1000ms = false;

    if (seed100ms != lastSeed100ms) {
        lastSeed100ms = seed100ms; // Update seed
        IT_100ms      = true;
    } else IT_100ms = false;

    if (seed10ms != lastSeed10ms) {
        lastSeed10ms = seed10ms; // Update seed
        IT_10ms      = true;
    } else IT_10ms = false;
}
void Time::updateMostRecentSwitchHitTime() {
    mostRecentSwitchHitTime = currentTime;
}
void Time::updateLastTiltWarningTime() {
    lastTiltWarningTime = currentTime;
}
void Time::updateBallEnteredTroughTime() {
    ballEnteredTroughTime = currentTime;
}
void Time::updateLastOutholeKickTime() {
    lastOutholeKickTime = currentTime;
}
void Time::updateLastScoringTime() {
    lastScoringTime = currentTime;
}
void Time::updateLastBonusPointsTime() {
    lastBonusPointsTime = currentTime;
}
void Time::updateLastTestCycleTime() {
    lastTestCycleTime = currentTime;
}
void Time::updateLastModeTimerUpdateTime() {
    lastModeTimerUpdateTime = currentTime;
}
void Time::updateLastSubModeTimerUpdateTime() {
    lastSubModeTimerUpdateTime = currentTime;
}
void Time::updateLastLampAnimUpdateTime() {
    lastLampAnimUpdateTime = currentTime;
}
void Time::updateLastAttractScoreUpdateTime() {
    lastAttractScoreUpdateTime = currentTime;
}
void Time::updateLastDisplayUpdateTime() {
    lastDisplayUpdateTime = currentTime;
}
void Time::updateLastScoreChangeTime() {
    lastScoreChangeTime = currentTime;
}

//
//  Reset
//

void Time::resetBallEnteredTroughTime() {
    ballEnteredTroughTime = 0;
}
void Time::resetLastOutholeKickTime() {
    lastOutholeKickTime = 0;
}
void Time::setLastScoringTime(Time::time_t time) {
    lastScoringTime = time;
}

//
//  Game specific
//

Time::time_t Time::getLastSaucerSwitchHitTime() {
    return lastSaucerSwitchHitTime;
}
void Time::updateLastSaucerSwitchHitTime() {
    lastSaucerSwitchHitTime = currentTime;
}
void Time::resetLastSaucerSwitchHitTime() {
    lastSaucerSwitchHitTime = 0;
}

Time::time_t Time::getLastSaucerKickTime() {
    return lastSaucerKickTime;
}
void Time::updateLastSaucerKickTime() {
    lastSaucerKickTime = currentTime;
}

Time::time_t Time::getLastUpperDropsResetTime() {
    return lastUpperDropsResetTime;
}
void Time::updateLastUpperDropsResetTime() {
    lastUpperDropsResetTime = currentTime;
}
