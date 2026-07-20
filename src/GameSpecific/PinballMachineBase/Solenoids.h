#ifndef RPU_BASE_SOLENOIDS_H
#define RPU_BASE_SOLENOIDS_H

//*************************************************************************************************************************************************************/
//
// SOLENOIDS
//
//*************************************************************************************************************************************************************/

#define SOL_CHIME_Q2 0       // Q2
#define SOL_CHIME_10 1       // Q1
#define SOL_CHIME_100 2      // Q5
#define SOL_CHIME_1000 3     // Q6
#define SOL_CHIME_10000 4    // Q7
#define SOL_KNOCKER 5        // Q3
#define SOL_OUTHOLE 6        // Q4
#define SOL_SAUCER 7         // Q8
#define SOL_LEFT_BUMPER 8    // Q13
#define SOL_RIGHT_BUMPER 9   // Q14
#define SOL_BOTTOM_BUMPER 10 // Q9
#define SOL_LEFT_SLING 11    // Q10
#define SOL_Q12 12           // Q12
#define SOL_RIGHT_SLING 13   // Q11
#define SOL_Q16 14           // Q16

#define NUMBER_OF_SOL 15 // Total number of solenoids
// End of defines for SOLENOIDS

// Define Solenoid Strength
#define SOL_KNOCKER_STRENGTH 2
#define SOL_OUTHOLE_KICKER_STRENGTH 3
#define SOL_SLING_STRENGTH 4
#define SOL_BUMPER_STRENGTH 4
#define SOL_CHIME_STRENGTH 2
#define SOL_SAUCER_STRENGTH 5
#define SOL_DROP_RESET_STRENGTH 8
#define SOL_DROP_STRENGTH 4
#define SOL_TEST_STRENGTH 5

// SWITCHES_WITH_TRIGGERS are for switches that will automatically
// activate a solenoid (like in the case of a chime that rings on a rollover)
// but SWITCHES_WITH_TRIGGERS are fully debounced before being activated
#define NUM_SWITCHES_WITH_TRIGGERS 5

// PRIORITY_SWITCHES_WITH_TRIGGERS are switches that trigger immediately
// (like for pop bumpers or slings) - they are not debounced completely
#define NUM_PRIORITY_SWITCHES_WITH_TRIGGERS 5
// End of SOLENOIDS

#endif // RPU_BASE_SOLENOIDS_H
