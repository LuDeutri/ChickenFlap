#ifndef SRC_UTIL_FILTER_H_
#define SRC_UTIL_FILTER_H_

#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Implement a simple low pass filter by using the moving average method.
 * a is the newer value and b the older.
 * Uses internally the calculation: a * (1 - w) + b * w.
 * The parameter w MUST be in the interval [0, 1].
 */
float lowPassFilter(float a, float b, float w);

/**
 * Implement a simple low pass filter by using the moving average method.
 * a is the newer value and b the older.
 * Uses internally the calculation: a * (1 - w) + b * w.
 * The parameter w MUST be in the interval [0, 1].
 */
uint16_t lowPassFilterUint16(uint16_t a, uint16_t b, float w);

#ifdef __cplusplus
}
#endif

#endif
