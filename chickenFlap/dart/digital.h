#ifndef DART_DIGITAL_H_
#define DART_DIGITAL_H_

/**
 * Defines the state of a logic low pin (meaning a logical 0).
 */
#define LOW false
/**
 * Defines the state of a logic high pin (meaning a logical 1).
 */
#define HIGH true

/**
 * Defines the mode of any input or output pin. Some options may not be supported by all hardware types.
 */
typedef enum {
    /**
     * Pin is set as an input without any pull-down or pull-up resistors.
     */
    ModeInput,
    /**
     * Pin is set as an input with an pull-down resistor.
     */
    ModeInputPullDown,
    /**
     * Pin is set as an input with an pull-up resistor.
     */
    ModeInputPullUp,
    /**
     * Pin is set as an output in push-pull configuration.
     */
    ModeOutput,
    /**
     * Pin is set as an output with open-drain configuration.
     */
    ModeOutputOpenDrain
} PinMode_t;

typedef void (*DigitalInterruptCallback)(const void* arg);

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the digital HAL.
 * MUST be called before any other digital function.
 * Implemented by the HAL wrapper.
 */
void dartDigital_init();

/**
 * Updates the digital system.
 * The digital HAL MUST be initialized see dartDigital_init().
 * Implemented by the HAL wrapper.
 */
void dartDigital_update();

/**
 * Sets the mode of the digital channel. If the pin mode is not possible on this hardware digitalMode will not fail and try to find the best mode.
 * The digital HAL MUST be initialized see dartDigital_init().
 * Implemented by the HAL wrapper.
 */
void digitalMode(int channel, PinMode_t mode);

/**
 * Reads the state of the digital channel.
 * channel: Index of the digital channel to be read.
 * Returns the state of the digital channel.
 * The digital HAL MUST be initialized see dartDigital_init().
 * Implemented by the HAL wrapper.
 */
bool digitalRead(int channel);

/**
 * Writes the state of the digital channel.
 * channel: Index of the digital channel to be read.
 * state: The state that should be written to the channel.
 * The digital HAL MUST be initialized see dartDigital_init().
 * Implemented by the HAL wrapper.
 */
void digitalWrite(int channel, bool state);

/**
 * Toggles the state of the digital channel.
 * channel: Index of the digital channel to be toggled.
 * The digital HAL MUST be initialized see dartDigital_init().
 * Implemented by the HAL wrapper.
 */
void digitalToggle(int channel);

/**
 * Sets the function which is called when the global digital interrupt is called.
 * May not be implemented by every hardware.
 */
void digitatSetInterruptCallback(DigitalInterruptCallback callback, void* arg);

#ifdef __cplusplus
}
#endif

#endif
