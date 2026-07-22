#ifndef GS_ADJUSTMENTS_H
#define GS_ADJUSTMENTS_H

#include "EEPROM.h"
#include "System/OperatorMenu/OperatorMenu.h"

#define EEPROM_ADJ_GRID_HELD 100

#define NUMBER_OF_GS_ADJ 1
const AdjItem specificAdjustmentTable[NUMBER_OF_GS_ADJ] = {
        {EEPROM_ADJ_GRID_HELD, TYPE_BOOL, ADJ_TOGGLE, 0, 1, 1, nullptr, 1, &holdGrid},
};

#endif // GS_ADJUSTMENTS_H
