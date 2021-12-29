/**
 * Wrapper around the ADC HAL. Implements functions for initializing the ADCs, updating them and getting the raw data.
 */

#ifndef DART_ANALOG_H_
#define DART_ANALOG_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"
#include "../../chickenFlap/dart/util/math.h"
#include "../../chickenFlap/dart/util/timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the analog HAL.
 * MUST be called before any other analog function.
 */
void dartAnalog_init();

/**
 * Updates the ADC data and copies the channel data to an local buffer.
 * The analog HAL MUST be initialized see dartAnalog_init().
 */
void dartAnalog_update();

/**
 * Returns the last saved raw ADC value for the specified channel. Raw values outside the interval of [0, ADC_MAX_VALUE] are clamped.
 * channel: The channel number of the ADC for which the value is returned. MUST be less than ADC_CHANNEL_COUNT.
 * Returned value is in interval [0, ADC_MAX_VALUE].
 * The analog HAL MUST be initialized see dartAnalog_init().
 */
uint16_t getADC(int channel);

/**
 * Maps the last saved raw ADC value for the specified channel from the range [0, ADC_MAX_VALUE] to [0, Vref].
 * channel: The channel number of the ADC for which the value is returned. MUST be less than ADC_CHANNEL_COUNT.
 * The analog HAL MUST be initialized see dartAnalog_init().
 */
float mapADCVoltage(int channel, float vref);

/**
 * Maps the last saved raw ADC value for the specified channel from the range [lower, upper]
 * to the interval [minValue, maxValue]. Ensuring the value is always in the correct bounds, meaning when the raw ADC value is smaller
 * than the parameter lower, the value minValue is returned. Is the raw ADC value larger then upper, then the value maxValue is returned.
 * channel: The channel number of the ADC for which the value is returned. MUST be less than ADC_CHANNEL_COUNT.
 * lower: Lower bound of the interval. 0 <= lower <= upper <= ADC_MAX_VALUE.
 * upper: Lower bound of the interval. 0 <= lower <= upper <= ADC_MAX_VALUE.
 * minValue MUST be less than or equal to maxValue.
 * The analog HAL MUST be initialized see dartAnalog_init().
 */
float mapADC(int channel, uint16_t lower, uint16_t upper, float minValue, float maxValue);

/**
 * Maps the raw ADC value given from the range [lower, upper]
 * to the interval [minValue, maxValue]. Ensuring the value is always in the correct bounds, meaning when the raw ADC value is smaller
 * than the parameter lower, the value minValue is returned. Is the raw ADC value larger then upper, then the value maxValue is returned.
 * value: The raw value of the ADC which should be mapped.
 * lower: Lower bound of the interval. 0 <= lower <= upper <= ADC_MAX_VALUE.
 * upper: Lower bound of the interval. 0 <= lower <= upper <= ADC_MAX_VALUE.
 * minValue MUST be less than or equal to maxValue.
 */
float mapADCValue(uint16_t value, uint16_t lower, uint16_t upper, float minValue, float maxValue);

/**
 * Initializes the HAL behind the ADC. Implemented by the HAL wrapper.
 */
void hal_adc_init();
/**
 * Updates the HAL behind the ADC. Implemented by the HAL wrapper.
 */
void hal_adc_update();

/**
 * Returns the measured ADC value on the specified channel. Implemented by the HAL wrapper.
 */
uint16_t hal_adc_get(int channel);

#ifdef __cplusplus
}
#endif

#endif
