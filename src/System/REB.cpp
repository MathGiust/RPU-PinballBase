//
// Created by mathias on 5/24/25.
//

#include "GameSpecific/RPU_Config.h"
#ifdef RPU_OS_USE_REB

#include "REB.h"

#include <Arduino.h>

//
//  REB COMMUNICATION
//

void REB_FlashGI() {
    uint8_t txbuf[4];
    txbuf[0] = REB_START_BYTE;
    txbuf[1] = 4;
    txbuf[2] = REB_FLASH_BB_GI;
    txbuf[3] = REB_END_BYTE;
    REB_Serial.write(txbuf, 4);
}
void REB_SetState(const bool state) {
    uint8_t txbuf[4];
    txbuf[0] = REB_START_BYTE;
    txbuf[1] = 4;
    if (state) txbuf[2] = REB_BB_GI_ON;
    else txbuf[2] = REB_BB_GI_OFF;
    txbuf[3] = REB_END_BYTE;
    REB_Serial.write(txbuf, 4);
}
void REB_TX(const uint8_t animationType) {
    uint8_t txbuf[4];
    txbuf[0] = REB_START_BYTE;
    txbuf[1] = 4;
    txbuf[2] = animationType;
    txbuf[3] = REB_END_BYTE;
    REB_Serial.write(txbuf, 4);
}

//
//  RPU Implementation
//

static Time::time_t GI_QueueTimes[REB_GI_FLASH_QUEUE_SIZE];
static uint8_t      GI_QueueTypes[REB_GI_FLASH_QUEUE_SIZE];

void REB::initREB() {
    REB_Serial.begin(115200);
}

void REB::update(const Time::time_t currentTime) {
    // Update state queue
    for (uint8_t i = 0; i < REB_GI_FLASH_QUEUE_SIZE; i++) {
        if (GI_QueueTimes[i] && GI_QueueTimes[i] < currentTime) {
            REB_TX(GI_QueueTypes[i]);
            GI_QueueTimes[i] = 0;
            GI_QueueTypes[i] = 0;
        }
    }
}
void REB::REB(uint8_t animationtype, Time::time_t whenToAnimate) {
    for (uint8_t i = 0; i < REB_GI_FLASH_QUEUE_SIZE; i++) {
        if (GI_QueueTimes[i]) continue;
        GI_QueueTimes[i] = whenToAnimate;
        GI_QueueTypes[i] = animationtype;
        break;
    }
}

#endif
