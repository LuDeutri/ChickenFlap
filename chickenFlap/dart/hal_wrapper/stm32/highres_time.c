#include "../../../../chickenFlap/dart/hal_wrapper/stm32/highres_time.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_HIGHRES_TIME)
void highres_time_init() {
	HAL_TIM_Base_Start(HIGHRES_TIMER);
}

uint64_t highres_time_get() {
	static uint64_t highres_timer = 0;
	static uint32_t lastCounter = 0;
	uint32_t counter = (HIGHRES_TIMER)->Instance->CNT;

	// Handle Overflow
	if (counter < lastCounter)
		highres_timer += (HIGHRES_TIMER_MAX_VALUE - lastCounter) + counter;
	else
		highres_timer += counter - lastCounter;
	lastCounter = counter;

	return highres_timer * HIGHRES_TIMER_FACTOR;
}
#endif
