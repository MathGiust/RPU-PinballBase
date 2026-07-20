#include "Lamps.h"

LAMPCOLL(
        descendingBonusLamps, 0,
);
LAMPCOLL(
        bonusMultiplier, 3,
        LAMP_2X_BONUS, LAMP_3X_BONUS, LAMP_4X_BONUS
);
LAMPCOLL(
        stars, 4,
        LAMP_LEFT_INLANE_STAR, LAMP_RIGHT_INLANE_STAR, LAMP_TOP_LEFT_STAR, LAMP_TOP_RIGHT_STAR
);
LAMPCOLL(
        starAwards, 2,
        LAMP_STARS_EXTRA_BALL, LAMP_STARS_SPECIAL
);
LAMPCOLL(
        captiveBall, 5,
        LAMP_CAPTIVE_BALL_2X, LAMP_CAPTIVE_BALL_3X, LAMP_CAPTIVE_BALL_4X, LAMP_CAPTIVE_BALL_SPECIAL, LAMP_CAPTIVE_BALL_25K
);
LAMPCOLL(
        leftDrop, 3,
        LAMP_LEFT_DROP_PYRAMID, LAMP_LEFT_DROP_ARROW, LAMP_LEFT_DROP_EYE
);
LAMPCOLL(
        centerDrop, 3,
        LAMP_CENTER_DROP_PYRAMID, LAMP_CENTER_DROP_ARROW, LAMP_CENTER_DROP_EYE
);
LAMPCOLL(
        rightDrop, 3,
        LAMP_RIGHT_DROP_PYRAMID, LAMP_RIGHT_DROP_ARROW, LAMP_RIGHT_DROP_EYE
);
LAMPCOLL(
        gridEyeTop, 3,
        LAMP_CENTER_EYE_1, LAMP_CENTER_EYE_2, LAMP_CENTER_EYE_3
);
LAMPCOLL(
        gridEyeMid, 3,
        LAMP_CENTER_EYE_4, LAMP_CENTER_EYE_5, LAMP_CENTER_EYE_6
);
LAMPCOLL(
        gridEyeBot, 3,
        LAMP_CENTER_EYE_7, LAMP_CENTER_EYE_8, LAMP_CENTER_EYE_9
);
LAMPCOLL(
        gridPyramidTop, 3,
        LAMP_CENTER_PYRAMID_1, LAMP_CENTER_PYRAMID_2, LAMP_CENTER_PYRAMID_3
);
LAMPCOLL(
        gridPyramidMid, 3,
        LAMP_CENTER_PYRAMID_4, LAMP_CENTER_PYRAMID_5, LAMP_CENTER_PYRAMID_6
);
LAMPCOLL(
        gridpyramidBot, 3,
        LAMP_CENTER_PYRAMID_7, LAMP_CENTER_PYRAMID_8, LAMP_CENTER_PYRAMID_9
);
LAMPCOLL(
        allEyes, 4,
        LAMP_EYE_BB, LAMP_CENTER_DROP_EYE, LAMP_LEFT_DROP_EYE, LAMP_RIGHT_DROP_EYE,
);
LAMPCOLL(
        allPyramids, 4,
        LAMP_PYRAMID_BB, LAMP_CENTER_DROP_PYRAMID, LAMP_LEFT_DROP_PYRAMID, LAMP_RIGHT_DROP_PYRAMID,
);

extern LampCollection* lampCollections[NUM_LAMP_COLLECTIONS] = {
    &descendingBonusLamps,
    &bonusMultiplier,
    &stars,
    &starAwards,
    &captiveBall,
    &leftDrop,
    &centerDrop,
    &rightDrop,
    &allEyes,
    &allPyramids,
    &gridEyeTop,
    &gridEyeMid,
    &gridEyeBot,
    &gridPyramidTop,
    &gridPyramidMid,
    &gridpyramidBot
};
