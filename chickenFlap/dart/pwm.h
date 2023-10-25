#ifndef DART_PWM_H_
#define DART_PWM_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the PWM HAL.
 * MUST be called before any other PWM function.
 * Implemented by the HAL wrapper.
 */
void dartPWM_init();

/**
 * Starts the PWM output.
 * The PWM HAL MUST be initialized see dartPWM_init().
 * Implemented by the HAL wrapper.
 */
void dartPWM_start(int channel);

/**
 * Stops the PWM output.
 * The PWM HAL MUST be initialized see dartPWM_init().
 * Implemented by the HAL wrapper.
 */
void dartPWM_stop(int channel);

/**
 * Configures the PWM output.
 * dutyCycle must be between 0 and 1.
 * The PWM HAL MUST be initialized see dartPWM_init().
 * Implemented by the HAL wrapper.
 */
void dartPWM_update(int channel, float dutyCycle, float frequency);

float dartPWM_read_frequency(int channel);


#ifdef __cplusplus
}
#endif

#endif
