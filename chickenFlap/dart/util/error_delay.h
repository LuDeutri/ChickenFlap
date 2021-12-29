#ifndef SRC_UTIL_BUBBLESORT_H_
#define SRC_UTIL_BUBBLESORT_H_

#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	util_time_t last_error_time;
} err_delay_t;

/**
 * Convient marco to reset the error delay.
 */
#define reset_error_delay(delay) (error_delay(delay, false, 0))

/**
 * Returns true only when isError is true more than timeout milliseconds.
 */
bool error_delay(err_delay_t* delay, bool isError, util_time_t timeout);

#ifdef __cplusplus
}
#endif

#endif
