/*********************************************************************


Utilities Header File
Version : 0.20


*********************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H

#include "MachineState.h"

#include <stdint.h>

namespace Utilities {
bool isNumberEven(int number);

// Arrays
void          resetArray(bool* array, uint8_t arraySize);
void          resetArray(uint8_t* array, uint8_t arraySize);
bool          shiftArray(bool* array, uint8_t arraySize);
bool          checkIfArrayIsTrue(const bool* array, uint8_t arraySize);
bool          checkIfByteOnList(const uint8_t* list, uint8_t listSize, uint8_t byte);
bool*         byteToArray(uint8_t byte, bool* array, uint8_t arraySize);
uint8_t       arrayToByte(const bool* array, uint8_t arraySize);
uint8_t       numberOfTrueElements(const bool* array, uint8_t arraySize);
uint8_t       getFirstTrueArray(const bool* array, uint8_t arraySize);
uint8_t       getFirstFalseArray(const bool* array, uint8_t arraySize);
uint8_t       getLastTrueArray(const bool* array, uint8_t arraySize);
score_t       getBooleanArrayAsScore(const bool* array, uint8_t arraySize);
unsigned long pow10(uint8_t power);

// Bytes
void setBit(uint8_t& byte, uint8_t bitToSet, bool bitValue);
bool getBit(const uint8_t& byte, uint8_t bitToGet);
} // namespace Utilities

#endif // UTILITIES_H
