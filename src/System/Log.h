#ifndef RPU_LOG_H
#define RPU_LOG_H

#pragma once

#include <HardwareSerial.h>
#include <stdint.h>

namespace Log {

enum LogIDs : uint8_t {
    LOG_NONE = 0,
    LOG_MACHINE_STATE,
    LOG_PLAYER_STATE,
    LOG_OPERATOR,
    LOG_LAMPS,
    LOG_SOLENOIDS,
    LOG_DISPLAY,
    LOG_SOUND,
    LOG_WTRIG,
    LOG_ATTRACT,
    LOG_GAMEPLAY,
    LOG_GAMESTATE,
    LOG_BONUS,
    LOG_GAMEMODE,
};

void printLogID(LogIDs logID);
void printMessage(LogIDs, const char* text);

template <typename T>
void printVariable(const LogIDs logID, const char* text, T variable);
} // namespace Log

#endif // RPU_LOG_H
