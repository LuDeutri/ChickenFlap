#include "../../../chickenFlap/dart/util/fixpoint.h"

int32_t convertToFixpoint(float value, uint8_t n) {
    // value * 2^n
    return (int32_t)(value * (1 << n));
}

float convertFromFixpoint(int32_t value, uint8_t n) {
    // value / 2^n
    return ((float)value) / (1 << n);
}
