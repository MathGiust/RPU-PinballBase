#ifndef RPU_BASE_SOLENOIDS_H
#define RPU_BASE_SOLENOIDS_H

#define SOL_LEFT_SLING 0                // Q2
#define SOL_RIGHT_SLING 1               // Q1
#define SOL_Q5 2                        // Q5
#define SOL_Q6 3                        // Q6
#define SOL_Q7 4                        // Q7
#define SOL_KNOCKER 5                   // Q3
#define SOL_OUTHOLE 6                   // Q4
#define SOL_SAUCER 7                    // Q8
#define SOL_LEFT_BUMPER 8               // Q13
#define SOL_RIGHT_BUMPER 9              // Q14
#define SOL_BOTTOM_BUMPER 10            // Q9
#define SOL_Q10 11                      // Q10
#define SOL_LEFT_DROP_TARGET_RESET 12   // Q12
#define SOL_RIGHT_DROP_TARGET_RESET 13  // Q11
#define SOL_CENTER_DROP_TARGET_RESET 14 // Q16

#define NUMBER_OF_SOL 15 // Total number of solenoids

// Define Solenoid Strength
#define SOL_KNOCKER_STRENGTH 2
#define SOL_OUTHOLE_KICKER_STRENGTH 3
#define SOL_SLING_STRENGTH 4
#define SOL_BUMPER_STRENGTH 4
#define SOL_CHIME_STRENGTH 2
#define SOL_SAUCER_STRENGTH 5
#define SOL_DROP_RESET_STRENGTH 12
#define SOL_DROP_STRENGTH 4
#define SOL_TEST_STRENGTH 5

// Debounced Solenoids (BALLY/STERN)
#define NUM_SWITCHES_WITH_TRIGGERS 5
#define NUM_PRIORITY_SWITCHES_WITH_TRIGGERS NUM_SWITCHES_WITH_TRIGGERS

#endif // RPU_BASE_SOLENOIDS_H
