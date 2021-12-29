#include "../../../../chickenFlap/dart/hal_wrapper/dummy/digital.h"

#ifdef HAL_DUMMY_GPIO

void dartDigital_init() {
	// Do nothing
}

void dartDigital_update() {
	// Do nothing
}

void digitalMode(int channel, PinMode_t mode) {
	// Do nothing
}

bool digitalRead(int channel) {
	return false; 
}

void digitalWrite(int channel, bool state) {
	// Do nothing
}

void digitalToggle(int channel) {
	// Do nothing
}

void digitatSetInterruptCallback(DigitalInterruptCallback callback, void* arg) {
    // Do nothing
}
#endif
