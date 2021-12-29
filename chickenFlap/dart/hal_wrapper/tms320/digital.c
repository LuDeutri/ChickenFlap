#include "../../../../chickenFlap/dart/hal_wrapper/tms320/digital.h"

#if defined(USE_TMS320) && defined(HAL_ENABLE_GPIO)

#ifndef DIGITAL_CHANNEL_COUNT
#error "DIGITAL_CHANNEL_COUNT is not defined"
#endif

void dartDigital_init() {
    Device_initGPIO();
}

void dartDigital_update() {
    // Nothing to do
}

void digitalMode(int channel, PinMode_t mode) {
    DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);

    switch(mode) {
    case ModeInput:
    case ModeInputPullDown:
        GPIO_setPadConfig(channel, GPIO_PIN_TYPE_STD);
        GPIO_setDirectionMode(channel, GPIO_DIR_MODE_IN);
        break;
    case ModeInputPullUp:
        GPIO_setPadConfig(channel, GPIO_PIN_TYPE_PULLUP);
        GPIO_setDirectionMode(channel, GPIO_DIR_MODE_IN);
        break;
    case ModeOutput:
        GPIO_setPadConfig(channel, GPIO_PIN_TYPE_STD);
        GPIO_setDirectionMode(channel, GPIO_DIR_MODE_OUT);
        break;
    case ModeOutputOpenDrain:
        GPIO_setPadConfig(channel, GPIO_PIN_TYPE_OD);
        GPIO_setDirectionMode(channel, GPIO_DIR_MODE_OUT);
        break;
    }
}

bool digitalRead(int channel) {
    DART_ASSERT_RETURN(channel >= 0, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, false);

    return GPIO_readPin(channel) != 0;
}

void digitalWrite(int channel, bool state) {
    DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);

    GPIO_writePin(channel, state);
}

void digitalToggle(int channel) {
    DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);

    GPIO_togglePin(channel);
}

void digitatSetInterruptCallback(DigitalInterruptCallback callback, const void* arg) {
    // Do nothing
}
#endif
