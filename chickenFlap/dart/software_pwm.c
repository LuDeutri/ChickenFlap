#include "../../chickenFlap/dart/software_pwm.h"

software_pwm_pin_t softwarePWM_init(int digitalPin, bool inverted) {
    software_pwm_pin_t pin;
    pin.digitalPin = digitalPin;
    pin.inverted = inverted;
    pin.lastRise = 0;
    pin.lastState = digitalRead(digitalPin);
    pin.duty = 0;
    pin.cycle = 0;

    if (inverted)
        pin.lastState = !pin.lastState;

    return pin;
}

void softwarePWM_update(software_pwm_pin_t* pin) {
    DART_NOT_NULL(pin, DART_ERROR_VALUE_IS_NULL);

    bool state = digitalRead(pin->digitalPin);
    if (pin->inverted)
        state = !state;

    // Edge detected?
    if (state != pin->lastState) {
        // Falling
        if (state == LOW) {
            if (pin->lastRise != 0)
                pin->duty = (millis() - pin->lastRise);
        }
        else { // or rising edge?
            if (pin->lastRise != 0)
               pin->cycle = (millis() - pin->lastRise);
            pin->lastRise = millis();
        }

        pin->lastState = state;
    }
}

float softwarePWM_getDuty(software_pwm_pin_t* pin) {
    if (pin->cycle <= 0)
        return 0;
    if (pin->duty >= pin->cycle)
        return 1;
    return (float)pin->duty / (float)pin->cycle;
}

float softwarePWM_getFrequency(software_pwm_pin_t* pin) {
    if (pin->cycle <= 0)
        return 0;
    return 1000.0f / (float)pin->cycle;
}
