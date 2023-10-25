#include "../../../../chickenFlap/dart/hal_wrapper/stm32/pwm.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_PWM)
TIM_HandleTypeDef* timerMap[TIMER_CHANNEL_COUNT] = TIMER_MAP;
uint16_t timerMapChannel[TIMER_CHANNEL_COUNT] = TIMER_MAP_CHANNEL;
uint16_t timerMaxPulse[TIMER_CHANNEL_COUNT] = TIMER_MAX_PULSE;
bool timerRunning[TIMER_CHANNEL_COUNT];

void dartPWM_init() {
	memset(timerRunning, 0, sizeof(timerRunning));
}

void dartPWM_start(int channel) {
	DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(channel < TIMER_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(timerMap[channel], DART_ERROR_INVALID_VALUE);

	HAL_TIM_PWM_Start(timerMap[channel], timerMapChannel[channel]);
	timerRunning[channel] = true;
}

void dartPWM_stop(int channel) {
	DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(channel < TIMER_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(timerMap[channel], DART_ERROR_INVALID_VALUE);

	HAL_TIM_PWM_Stop(timerMap[channel], timerMapChannel[channel]);
	timerRunning[channel] = false;
}

void dartPWM_update(int channel, float dutyCycle, float frequency) {
	DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(channel < TIMER_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(dutyCycle >= 0 && dutyCycle <= 1, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(timerMap[channel], DART_ERROR_INVALID_VALUE);

	DART_NOT_USED(frequency);

	bool running = timerRunning[channel];
	if (running){
		dartPWM_stop(channel);
		running = timerRunning[channel];
	}

	TIM_OC_InitTypeDef init;
	init.Pulse = (uint16_t)(timerMaxPulse[channel] * dutyCycle);
	init.OCMode = TIM_OCMODE_PWM1;
	init.OCPolarity = TIM_OCPOLARITY_HIGH;
	init.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(timerMap[channel], &init, timerMapChannel[channel]);

	if (!running)
		dartPWM_start(channel);
}

float dartPWM_read_frequency(int channel) {
	DART_ASSERT_RETURN(channel >= 0, DART_ERROR_INVALID_VALUE, 0);
	DART_ASSERT_RETURN(channel < TIMER_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, 0);
	DART_NOT_NULL_RETURN(timerMap[channel], DART_ERROR_INVALID_VALUE, 0);

	uint16_t value = HAL_TIM_ReadCapturedValue(timerMap[channel], timerMapChannel[channel]);
	if (value == 0)
		return value;
	float clock = (HAL_RCC_GetPCLK1Freq() / 4.0f);
	return (float)(clock / value);
}



#endif
