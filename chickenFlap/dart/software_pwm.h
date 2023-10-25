#ifndef DART_SOFTWARE_PWM_H_
#define DART_SOFTWARE_PWM_H_

#include "../../chickenFlap/dart/digital.h"
#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"
#include "../../chickenFlap/dart/util/time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int digitalPin;
    bool inverted;
    bool lastState;
    util_time_t lastRise;
    util_time_t duty;
    util_time_t cycle;
} software_pwm_pin_t;

/**
 * Initializes the software PWM pin and ties it to the given digital pin.
 */
software_pwm_pin_t softwarePWM_init(int digitalPin, bool inverted);

/**
 * Updates the PWM signal for the given pin.
 * softwarePWM_init MUST be used to initialize the pin.
 */
void softwarePWM_update(software_pwm_pin_t* pin);

/**
 * Returns the duty cycle of the PWM pin in percentage in the interval [0, 1].
 */
float softwarePWM_getDuty(software_pwm_pin_t* pin);

/**
 * Returns the frequency of the PWM pin in Hz.
 */
float softwarePWM_getFrequency(software_pwm_pin_t* pin);

#ifdef __cplusplus
}
#endif

#endif
