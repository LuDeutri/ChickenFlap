#include "../../../../chickenFlap/dart/hal_wrapper/dummy/pwm.h"

#if defined(HAL_DUMMY_PWM)

void dartPWM_init() {
}

void dartPWM_start(int channel) {

}

void dartPWM_stop(int channel) {

}

void dartPWM_update(int channel, float dutyCycle, float frequency) {

}

float dartPWM_read_frequency(int channel) {
	return 0;
}
#endif
