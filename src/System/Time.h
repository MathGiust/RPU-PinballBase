/*********************************************************************


Time Header File
    Version : 1.00


*********************************************************************/

#ifndef TIME_H
#define TIME_H

namespace Time {

typedef unsigned long time_t;

bool   get1000msIT();
bool   get100msIT();
bool   get10msIT();
time_t getElapsedTime(time_t time);

time_t getCurrentTime();
time_t getMostRecentSwitchHitTime();
time_t getLastTiltWarningTime();
time_t getBallEnteredTroughTime();
time_t getLastOutholeKickTime();
time_t getLastScoringTime();
time_t getLastBonusPointsTime();
time_t getLastTestCycleTime();
time_t getLastModeTimerUpdateTime();
time_t getLastSubModeTimerUpdateTime();
time_t getLastLampAnimUpdateTime();
time_t getLastAttractScoreUpdateTime();
time_t getLastDisplayUpdateTime();
time_t getLastScoreChangeTime();

void updateCurrentTime();
void updateMostRecentSwitchHitTime();
void updateLastTiltWarningTime();
void updateBallEnteredTroughTime();
void updateLastOutholeKickTime();
void updateLastScoringTime();
void updateLastBonusPointsTime();
void updateLastTestCycleTime();
void updateLastModeTimerUpdateTime();
void updateLastSubModeTimerUpdateTime();
void updateLastLampAnimUpdateTime();
void updateLastAttractScoreUpdateTime();
void updateLastDisplayUpdateTime();
void updateLastScoreChangeTime();

void resetBallEnteredTroughTime();
void resetLastOutholeKickTime();
void setLastScoringTime(Time::time_t time);

// Game specific
time_t getLastSaucerSwitchHitTime();
void   updateLastSaucerSwitchHitTime();
void   resetLastSaucerSwitchHitTime();

time_t getLastSaucerKickTime();
void   updateLastSaucerKickTime();

time_t getLastUpperDropsResetTime();
void   updateLastUpperDropsResetTime();

} // namespace Time

#endif
