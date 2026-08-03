/*********************************************************************


Utilities Source File
Version : 0.20


*********************************************************************/

#include "Utilities.h"

bool Utilities::isNumberEven(int number) {
    return !(number % 2);
}

//
//  Arrays
//

void Utilities::resetArray(bool* array, const uint8_t arraySize) {
    for (unsigned int i = 0; i < arraySize; i++)
        array[i] = false;
}
void Utilities::resetArray(uint8_t* array, const uint8_t arraySize) {
    for (unsigned int i = 0; i < arraySize; i++)
        array[i] = 0;
}
bool Utilities::shiftArray(bool* array, const uint8_t arraySize) {
    if (arraySize <= 1) return false; // No shift needed for empty or single-element arrays

    bool last = array[arraySize - 1]; // store last element

    // Shift all elements to the right
    for (int i = arraySize - 1; i > 0; --i) {
        array[i] = array[i - 1];
    }

    // Place the last element at the front
    array[0] = last;
    if (last) return true;
    return false;
}

bool Utilities::checkIfArrayIsTrue(const bool* array, const uint8_t arraySize) {
    for (unsigned int i = 0; i < arraySize; i++)
        if (!array[i]) return false;
    return true;
}
bool Utilities::checkIfByteOnList(const uint8_t* list, uint8_t listSize, uint8_t byte) {
    for (uint8_t i = 0; i < listSize; i++) {
        if (list[i] == byte) return true;
    }
    return false;
}

bool* Utilities::byteToArray(const uint8_t byte, bool* array, const uint8_t arraySize) {
    for (uint8_t bitInCheck = 0; bitInCheck < arraySize; bitInCheck++) {
        array[bitInCheck] = byte & 1 << bitInCheck;
    }
    return array;
}
uint8_t Utilities::arrayToByte(const bool* array, const uint8_t arraySize) {
    if (arraySize > 8) return 0;
    uint8_t returnByte = 0;
    for (uint8_t i = 0; i < arraySize; i++) {
        returnByte = returnByte | (*(array + i) ? 1 : 0) << i;
    }
    return returnByte;
}

uint8_t Utilities::numberOfTrueElements(const bool* array, const uint8_t arraySize) {
    uint8_t returnValue = 0;
    for (uint8_t i = 0; i < arraySize; i++) {
        if (array[i]) returnValue++;
    }
    return returnValue;
}
uint8_t Utilities::getFirstTrueArray(const bool* array, const uint8_t arraySize) {
    for (uint8_t i = 0; i < arraySize; i++) {
        if (array[i]) return i;
    }
    return 0xFF;
}
uint8_t Utilities::getLastTrueArray(const bool* array, const uint8_t arraySize) {
    for (uint8_t i = 0; i < arraySize; i++) {
        if (array[arraySize - i - 1]) return arraySize - 1 - i;
    }
    return 0xFF;
}
uint8_t Utilities::getFirstFalseArray(const bool* array, const uint8_t arraySize) {
    for (uint8_t i = 0; i < arraySize; i++) {
        if (!array[i]) return i;
    }
    return 0xFF;
}
score_t Utilities::getBooleanArrayAsScore(const bool* array, const uint8_t arraySize) {
    // Convert the binary string into an integer value
    score_t value = 0;
    for (int i = arraySize; i >= 0; i--) {
        value = value * 10 + array[i]; // Convert bin to digit
    }
    return value;
}
unsigned long Utilities::pow10(const uint8_t power) {
    if (!power) return 1;
    unsigned long returnValue = 10;
    for (uint8_t i = 0; i < power - 1; i++) {
        returnValue *= 10;
    }
    return returnValue;
}

//
//  Bytes
//

void Utilities::setBit(uint8_t& byte, uint8_t bitToSet, bool bitValue) {
    if (bitValue == true) byte |= (1 << bitToSet);
    else byte &= ~(1 << bitToSet);
}
bool Utilities::getBit(const uint8_t& byte, const uint8_t bitToGet) {
    return byte & (1 << bitToGet);
}
