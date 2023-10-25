#include "../../chickenFlap/dart/analog.h"

util_timer_t adcTimer;

void dartAnalog_init() {
	adcTimer = createTimer(2, 0);
	hal_adc_init();
}

void dartAnalog_update() {
	// Sample timer
	if (!hasTimerElapsed(&adcTimer))
		return;

	hal_adc_update();
}

uint16_t getADC(int channel) {
	DART_ASSERT_RETURN(channel >= 0, DART_ERROR_INVALID_VALUE, 0);
    DART_ASSERT_RETURN(channel < ADC_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, 0);

    // Ensure the value is within excepted bounds
    return min(hal_adc_get(channel), ADC_MAX_VALUE);
}

float mapADCVoltage(int channel, float vref) {
	return mapADC(channel, 0, ADC_MAX_VALUE, 0, vref);
}

float mapADC(int channel, uint16_t lower, uint16_t upper, float minValue, float maxValue) {
	return mapADCValue(getADC(channel), lower, upper, minValue, maxValue);
}

float mapADCValue(uint16_t value, uint16_t lower, uint16_t upper, float minValue, float maxValue) {
    DART_ASSERT_RETURN(upper <= ADC_MAX_VALUE, DART_ERROR_INVALID_VALUE, minValue);
    DART_ASSERT_RETURN(lower <= upper, DART_ERROR_INVALID_VALUE, minValue);
    DART_ASSERT_RETURN(minValue <= maxValue, DART_ERROR_INVALID_VALUE, minValue);

    // value is in [0, ADC_MAX_VALUE]

    // Check bounds
    // If the ADC is already out of bounds [lower, upper], then
    // there is no need for the linear map function, otherwise
    // invalid values are calculated.
    if (value <= lower)
        return minValue;
    else if (value >= upper)
        return maxValue;
    // Linear map function
    // 1. Map value to [0, 1] via lower and upper bounds,
    // 2. Multiply value ([0, 1]) with the range (maxValue - minValue) and add minValue.
    //    Maps the value to the interval [minValue, maxValue].
    return ((value - lower) / (float)(upper - lower)) * (maxValue - minValue) + minValue;
}
