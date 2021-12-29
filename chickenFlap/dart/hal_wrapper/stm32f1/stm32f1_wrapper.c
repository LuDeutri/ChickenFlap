#include "../../../../chickenFlap/dart/hal_wrapper/stm32f1/stm32f1_wrapper.h"

#ifdef USE_STM32F1
void hal_init() {

}

#ifdef HAL_ENABLE_TIME
uint32_t hal_get_millis() {
	return HAL_GetTick();
}
#endif

void hal_yield() {
#ifdef HAL_ENABLE_FREERTOS
	osThreadYield();
#endif
}

void hal_system_reset() {
	HAL_NVIC_SystemReset();
}

size_t hal_get_free_heap() {
#ifdef HAL_ENABLE_FREERTOS
	return (size_t)xPortGetFreeHeapSize();
#else
	return 0;
#endif
}

#endif
