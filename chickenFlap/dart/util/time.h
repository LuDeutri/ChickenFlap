/**
 * Implements common time based helper functions.
 */

#ifndef SRC_UTIL_TIME_H_
#define SRC_UTIL_TIME_H_

#include <stdint.h>

typedef uint32_t util_time_t;

#include "../../../chickenFlap/dart/standard_include.h"
#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returns the elapsed time in milliseconds since firmware start.
 */
util_time_t millis();

/**
 * Returns an boolean indicating whether an period beginning at start has elapsed.
 * @param start Time in millisecond where the period has started.
 * @param period Period in millisecond.
 * Returns true when the period has elapsed, otherwise false.
 */
bool hasTimeElapsed(util_time_t start, util_time_t period);

/**
 * Busy-waits (means the function blocks) for given time in milliseconds. Under the hood millis() is used.
 * CAN ONLY be called after initTime();
 */
void delayMillis(util_time_t time);

/**
 * Sets the scale for millis, which can be used to slow down or speed up time.
 * @param scale The scale to use for millis.
 */
void millisSetScale(float scale);

/**
 * Calls the HAL to return the internal timer.
 */
uint32_t hal_get_millis();

#ifdef __cplusplus
}
#endif

#endif
