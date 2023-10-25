#include "../../../chickenFlap/dart/util/filter.h"

float lowPassFilter(float a, float b, float w) {
    DART_ASSERT_RETURN(w >= 0, DART_ERROR_UTIL, b);
    DART_ASSERT_RETURN(w <= 1, DART_ERROR_UTIL, b);
    return a * (1.0f - w) + b * w;
}

uint16_t lowPassFilterUint16(uint16_t a, uint16_t b, float w) {
    return (uint16_t)lowPassFilter((float)a, (float)b, w);
}
