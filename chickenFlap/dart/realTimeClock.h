#ifndef DART_REALTIMECLOCK_H_
#define DART_REALTIMECLOCK_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/datetime.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the RTC HAL.
 * MUST be called before any other digital function.
 * Implemented by the HAL wrapper.
 */
void dartRTC_init();

/**
 * Updates the RTC system.
 * The digital HAL MUST be initialized see dartRTC_init().
 * Implemented by the HAL wrapper.
 */
void dartRTC_update();

/**
 * Reads the date and time of the RTC.
 * The digital HAL MUST be initialized see dartRTC_init().
 * Implemented by the HAL wrapper.
 */
datetime_t dartRTC_getDateTime();

/**
 * Writes the date and time of the RTC.
 * The digital HAL MUST be initialized see dartRTC_init().
 * Implemented by the HAL wrapper.
 */
void dartRTC_setDateTime(datetime_t datetime, bool summerTime);

#ifdef __cplusplus
}
#endif

#endif
