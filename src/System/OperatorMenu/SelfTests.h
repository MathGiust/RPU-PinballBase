#ifndef RPU_SELF_TESTS_H
#define RPU_SELF_TESTS_H

#include "OperatorMenu.h"

#include <stdint.h>

#define NUMBER_OF_STD_TESTS 5

namespace SelfTests {
void init(OperatorMenu* _operatorMenu);
void run();
void handleSwitchHit(uint8_t _switchHit);
} // namespace SelfTests

#endif // RPU_SELF_TESTS_H
