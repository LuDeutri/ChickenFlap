#include "../../../chickenFlap/dart/highlevel/LookupTable.h"

float LookupTable::lerp(float a, float b, float w) {
    if (w <= 0)
        return a;
    if (w >= 1)
        return b;
    return a * (1 - w) + b * w;
}

float LookupTable::lookupXY(const float* x, const float* y, size_t length, float v) {
    if (length == 0)
        return 0;

    if (v < x[0])
        return y[0];
    else if (v >= x[length - 1])
        return y[length - 1];

    for (size_t i = 1; i < length; i++) {
        float xr = x[i];

        if (v < xr) {
            float xl = x[i - 1];


            float yl = y[i - 1];
            float yr = y[i];

            float p = (v - xr) / (xr - xl);
            return lerp(yl, yr, p);
        }
    }
    return y[0];
}

float LookupTable::lookupXYFindX(const float* x, const float* y, size_t length, float v) {
    if (length == 0)
        return 0;
    if (length == 1)
        return x[0];

    float bestFitX = 0;
    float bestFitError = MAXFLOAT;
    for (size_t i = 0; i < length - 1; i++) {
        float yl = y[i];
        float yr = y[i + 1];
        if ((v >= yl && v <= yr) || (v <= yl && v >= yr)) {
            float xl = x[i];
            float xr = x[i + 1];
            float h = max(0.1f, fabsf(xl - xr) * 0.1f);

            while (xl < xr) {
                float currentY = lookupXY(x, y, length, xl);
                float error = std::abs(currentY - v);
                if (error < bestFitError) {
                    bestFitX = xl;
                    bestFitError = error;
                }
                xl += h;
            }
        }
    }
    return bestFitX;
}
