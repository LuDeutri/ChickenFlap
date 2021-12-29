#ifndef DART_HIGHRES_TIME_H_
#define DART_HIGHRES_TIME_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the high resolution timer.
 */
void highres_time_init();

/**
 *  Returns the high resolution timer in microseconds.
 */
uint64_t highres_time_get();

/**
 * Returns the high resolution timer in milliseconds.
 */
float highres_time_getMillis();

#ifdef __cplusplus
}
#endif

#endif
