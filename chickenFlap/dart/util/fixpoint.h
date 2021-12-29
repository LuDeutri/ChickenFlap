#ifndef SRC_UTIL_FIXPOINT_H_
#define SRC_UTIL_FIXPOINT_H_

#include "../../../chickenFlap/dart/util/math.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Implements functions to convert from and to fixed point numbers.
 * See https://en.wikipedia.org/wiki/Q_(number_format).
 */

/**
 * Converts the float to an int32 in the fixed point format Q16.n.
 */
int32_t convertToFixpoint(float value, uint8_t n);


/**
 * Converts the int32 in fixed point format Q16.n to a float.
 */
float convertFromFixpoint(int32_t value, uint8_t n);

#ifdef __cplusplus
}
#endif

#endif
