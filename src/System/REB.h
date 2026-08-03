#ifndef REB_H
#define REB_H

#include "GameSpecific/RPU_Config.h"
#ifdef RPU_OS_USE_REB

#include "Time.h"

#include <Arduino.h>

#define REB_Serial Serial2

#define REB_START_BYTE 0xAA
#define REB_MAX_MESSAGE_LENGHT 4
#define REB_END_BYTE 0xBB

#define REB_PF_GI_OFF 0x10
#define REB_PF_GI_ON 0x11
#define REB_FLASH_PF_GI 0x12

#define REB_BB_GI_OFF 0x20
#define REB_BB_GI_ON 0x21
#define REB_FLASH_BB_GI 0x22

#define REB_BOTH_GI_OFF 0x30
#define REB_BOTH_GI_ON 0x31
#define REB_FLASH_BOTH_GI 0x32

#define REB_NON_INVERTED_FLIPPERS 0x50
#define REB_INVERTED_FLIPPERS 0x51

#define REB_GI_FLASH_QUEUE_SIZE 16

namespace REB {
void initREB();
void REB(uint8_t animationtype, Time::time_t whenToAnimate);
void update(Time::time_t currentTime);
} // namespace REB

#endif
#endif // REB_H
