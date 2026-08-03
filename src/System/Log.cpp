#include "Log.h"

#include "MachineState.h"

#include <HardwareSerial.h>

void Log::printLogID(LogIDs logID) {
    {
        switch (logID) {
        default:
        case LOG_NONE:
            Serial.print("LOG : ");
            break;
        case LOG_MACHINE_STATE:
            Serial.print("MACHINE STATE : ");
            break;
        case LOG_PLAYER_STATE:
            Serial.print("PLAYER STATE : ");
            break;
        case LOG_OPERATOR:
            Serial.print("OPERATOR : ");
            break;
        case LOG_LAMPS:
            Serial.print("LAMPS : ");
            break;
        case LOG_DISPLAY:
            Serial.print("DISPLAY : ");
            break;
        case LOG_SOUND:
            Serial.print("SOUND : ");
            break;
        case LOG_WTRIG:
            Serial.print("WTRIG : ");
            break;
        case LOG_GAMEPLAY:
            Serial.print("GAMEPLAY : ");
            break;
        case LOG_GAMEMODE:
            Serial.print("GAMEMODE : ");
            break;
        case LOG_ATTRACT:
            Serial.print("ATTRACT : ");
            break;
        case LOG_GAMESTATE:
            Serial.print("GAMESTATE : ");
            break;
        case LOG_BONUS:
            Serial.print("BONUS : ");
            break;
        case LOG_SOLENOIDS:
            Serial.print("SOLENOIDS : ");
            break;
        }
    }
}

void Log::printMessage(const LogIDs logID, const char* text) {
    printLogID(logID);
    Serial.println(text);
}

template <typename T>
void Log::printVariable(const LogIDs logID, const char* text, T variable) {
    printLogID(logID);
    Serial.print(text);
    Serial.print(" : ");
    Serial.println(variable);
}

template void Log::printVariable<bool>(const LogIDs logID, const char* text, bool variable);
template void Log::printVariable<uint8_t>(const LogIDs logID, const char* text, uint8_t variable);
template void Log::printVariable<unsigned int>(const LogIDs logID, const char* text, unsigned int variable);
template void Log::printVariable<unsigned long>(const LogIDs logID, const char* text, unsigned long variable);
