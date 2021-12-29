#pragma once

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/util/math.h"

class LookupTable {
public:
    static float lerp(float a, float b, float w);
    static float lookupXY(const float* x, const float* y, size_t length, float v);
    static float lookupXYFindX(const float* x, const float* y, size_t length, float v);
};
