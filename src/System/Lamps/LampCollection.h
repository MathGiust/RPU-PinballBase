#ifndef RPU_LAMP_COLLECTION_H
#define RPU_LAMP_COLLECTION_H

#include "System/Time.h"

#include <stdint.h>

class LampCollection {

private:
    // Collection
    const uint8_t  collectionSize;
    const uint8_t* lamps;

public:
    constexpr LampCollection(const uint8_t size, const uint8_t* array)
        : collectionSize(size),
          lamps(array),
          animationRunning(false),
          animationType(0),
          animationParameter(0),
          animationStepTime(0),
          animationStartTime(0),
          animationEndTime(0) {};

    uint8_t getSize() const {
        return collectionSize;
    }
    const uint8_t* getLamps() const {
        return lamps;
    }

    // Animation
    bool         animationRunning;
    uint8_t      animationType;
    uint8_t      animationParameter;
    Time::time_t animationStepTime;
    Time::time_t animationStartTime;
    Time::time_t animationEndTime;
};

#define LAMPCOLL(name, size, ...)                            \
    static const uint8_t  name##_data[size] = {__VA_ARGS__}; \
    static LampCollection name(size, name##_data);

#endif // RPU_LAMP_COLLECTION_H
