/**
 *  math.h defines common helper math functions, that are useful in many cases in many different code locations.
 */

#ifndef SRC_UTIL_MATH_H_
#define SRC_UTIL_MATH_H_

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

/**
 * Defines the math constant PI (3.141...) as a float.
 */
#define PI (3.1415926535897932384626433832795028841971693993751058209749445923078164062f)
/**
 * Defines the math constant 2 * PI (6.282...) as a float. It is equal to 2 * PI.
 */
#define TWO_PI (PI * 2.0f)

#ifndef max
/**
 * Defines a max function with two arguments (a, b). When a is larger than b, then a is returned, otherwise b is returned. See also min(a, b).
 */
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
/**
 * Defines a min function with two arguments (a, b). When a is smaller than b, then a is returned, otherwise b is returned. See also max(a, b).
 */
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

/**
 * Defines a clamp function with three arguments (a, minValue, maxValue). Parameter a is clamped to be within the
 * interval [minValue, maxValue].
 * When a is smaller than minValue, minValue is returned.
 * When a is larger than maxValue, maxValue is returned.
 * When a is within [minValue, maxValue], a is returned.
 */
#define clamp(a, minValue, maxValue) (max(minValue, min(maxValue, (a))))

/**
 * Defines a within function. Checks if the parameter a is within the interval [minValue, maxValue].
 * Returns true when a is within [minValue, maxValue], otherwise false is returned.
 */
#define within(a, minValue, maxValue) ((a) >= minValue && (a) <= maxValue)

/**
 * Defines a lower_byte function. Returns the lower half of an int16 (meaning the last 8 bits).
 */
#define lower_byte(a) ((uint8_t)((a) & 0xFF))

/**
 * Defines a upper_byte function. Returns the upper half of an int16 (meaning the first 8 bits).
 */
#define upper_byte(a) ((uint8_t)(((a) >> 8) & 0xFF))

/**
 * Converts the value in degrees [0, 360] to radiant [0, TWO_PI].
 * See rad_to_deg.
 */
#define deg_to_rad(value) (((value) / 180.0f) * PI)

/**
 * Converts the value in radiant [0, TWO_PI] to degrees [0, 360].
 * See deg_to_rad.
 */
#define rad_to_deg(value) (((value) / PI) * 180.0f)

/**
 * Converts the value from interval [0, 1] to [0, 100].
 */
#define to_percentage_100(value) (clamp((uint8_t)ceil((value) * 100), 0, 100))

#define is_valid_float(value) (!isnanf(value) && !isinff(value))

#endif
