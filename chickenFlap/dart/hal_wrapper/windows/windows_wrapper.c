#include "../../../../chickenFlap/dart/hal_wrapper/windows/windows_wrapper.h"

#ifdef USE_WINDOWS
void hal_init() {

}

#ifdef HAL_ENABLE_TIME
uint32_t hal_get_millis() {
	static uint32_t hal_time_millis_start = 0;
	if (hal_time_millis_start == 0)
		hal_time_millis_start = chrono_time_millis();
	return chrono_time_millis() - hal_time_millis_start;
}
#endif

void hal_yield() {

}

size_t hal_get_free_heap() {
	return 0;
}

#endif
