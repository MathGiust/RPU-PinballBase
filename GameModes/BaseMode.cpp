#include "BaseMode.h"

uint8_t       BaseMode::globalTimer   = 0;
MachineState* BaseMode::machineState  = nullptr;
PlayerState*  BaseMode::currentPlayer = nullptr;
PlayerState*  BaseMode::players       = nullptr;
