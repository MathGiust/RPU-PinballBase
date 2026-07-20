#ifndef GS_BASE_LAMPS_H
#define GS_BASE_LAMPS_H

#include "System/Lamps/LampCollection.h"

#include <stdint.h>

// COL 1
#define LAMP_BONUS_1K 0 // Q14
#define LAMP_BONUS_2K 1 // Q29
#define LAMP_BONUS_3K 2 // Q36
#define LAMP_BONUS_4K 3 // Q57
#define LAMP_BONUS_5K 4 // Q12
#define LAMP_BONUS_6K 5 // Q27
#define LAMP_BONUS_7K 6 // Q38
#define LAMP_BONUS_8K 7 // Q50

// COL 2
#define LAMP_BONUS_9K 8   // Q13
#define LAMP_BONUS_10K 9  // Q28
#define LAMP_BONUS_20K 10 // Q44
#define LAMP_SPINNER 11   // Q51
#define LAMP_SPARE_R 12   // Q08
#define LAMP_SPARE_S 13   // Q54
#define LAMP_SPARE_P 14   // Q49
#define LAMP_SPARE_A 15   // Q35

// COL 3
#define LAMP_SPARE_E 16 // Q09
#define LAMP_LANE_B 17  // Q34
#define LAMP_LANE_C 18  // Q48
#define LAMP_LANE_A 19  // Q55
#define LAMP_STAR_1 20  // Q10
#define LAMP_STAR_2 21  // Q22
#define LAMP_STAR_3 22  // Q37
#define LAMP_STAR_4 23  // Q60

// COL 4
#define LAMP_STAR_5 24   // Q11
#define LAMP_STAR_6 25   // Q26
#define LAMP_STAR_7 26   // Q32
#define LAMP_STAR_8 27   // Q59
#define LAMP_STAR_9 28   // Q04
#define LAMP_STAR_10 29  // Q25
#define LAMP_3X_BONUS 30 // Q20
#define LAMP_2X_BONUS 31 // Q58

// COL 5
#define LAMP_STRIKE_5K 32      // Q01
#define LAMP_STRIKE_10K 33     // Q24
#define LAMP_STRIKE_15K 34     // Q42
#define LAMP_STRIKE_20K 35     // Q56
#define LAMP_STRIKE_30K 36     // Q02
#define LAMP_STRIKE_SPECIAL 37 // Q17
#define LAMP_BOTTOM_BUMPER 38  // Q41
#define LAMP_LANE_B_STRIKE 39  // Q46

// COL 6
#define LAMP_SHOOT_AGAIN_BB 40   // Q03
#define LAMP_MATCH 41            // Q23
#define LAMP_SHOOT_AGAIN_PF 42   // Q40
#define LAMP_CREDIT_INDICATOR 43 // Q52
#define LAMP_ABC_SPECIAL 44      // Q07
#define LAMP_SPARE_SPECIAL 45    // Q21
#define LAMP_5X_BONUS 46         // Q39
#define LAMP_ABC_5K 47           // Q53

// COL 7
#define LAMP_BALL_IN_PLAY 48  // Q16
#define LAMP_HIGH_SCORE 49    // Q15
#define LAMP_GAME_OVER 50     // Q33
#define LAMP_TILT 51          // Q47
#define LAMP_LEFT_OUTLANE 52  // Q05
#define LAMP_RIGHT_OUTLANE 53 // Q18
#define LAMP_LEFT_INLANE 54   // Q30
#define LAMP_RIGHT_INLANE 55  // Q43

// COL 8
#define LAMP_PLAYER_UP_1 56 // Q06
#define LAMP_PLAYER_UP_2 57 // Q19
#define LAMP_PLAYER_UP_3 58 // Q31
#define LAMP_PLAYER_UP_4 59 // Q45

enum LampCollectionIDs : uint8_t {
    NUM_LAMP_COLLECTIONS
};

extern LampCollection* lampCollections[NUM_LAMP_COLLECTIONS];

#endif // GS_BASE_LAMPS_H
