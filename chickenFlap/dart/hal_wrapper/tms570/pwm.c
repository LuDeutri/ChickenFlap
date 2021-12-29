#include "../../../../chickenFlap/dart/hal_wrapper/tms570/pwm.h"

#if defined(USE_TMS570) && defined(HAL_ENABLE_PWM)

#ifndef PWM_CHANNEL_COUNT
#error "PWM_CHANNEL_COUNT is not defined"
#endif

#ifndef PWM_MAP
#error "PWM_MAP is not defined"
#endif

#ifndef PWM_CHANNEL_MAP
#error "PWM_CHANNEL_MAP is not defined"
#endif


hetRAMBASE_t* pwmHandles[PWM_CHANNEL_COUNT] = PWM_MAP;
uint8_t pwmChannels[PWM_CHANNEL_COUNT] = PWM_CHANNEL_MAP;

void dartPWM_init() {
    // Done by digital module
}

void dartPWM_start(int channel) {
    DART_ASSERT(channel >= 0 && channel < PWM_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(pwmHandles[channel], DART_ERROR_INVALID_VALUE);
    pwmStart(pwmHandles[channel], pwmChannels[channel]);
}

void dartPWM_stop(int channel) {
    DART_ASSERT(channel >= 0 && channel < PWM_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(pwmHandles[channel], DART_ERROR_INVALID_VALUE);
    pwmStop(pwmHandles[channel], pwmChannels[channel]);
}

void dartPWM_update(int channel, float dutyCycle, float frequency) {
    DART_ASSERT(channel >= 0 && channel < PWM_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(pwmHandles[channel], DART_ERROR_INVALID_VALUE);
    DART_ASSERT(dutyCycle >= 0 && dutyCycle <= 1, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(frequency > 0, DART_ERROR_INVALID_VALUE);

    hetSIGNAL_t signal;
    signal.duty = dutyCycle;
    signal.period = (1.0 / (double)frequency) * 1000000.0;

    pwmSetSignal(pwmHandles[channel], pwmChannels[channel], signal);
}
#endif
