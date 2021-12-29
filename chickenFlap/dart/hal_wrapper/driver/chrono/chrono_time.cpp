#include "../../../../../chickenFlap/dart/hal_wrapper/driver/chrono/chrono_time.h"

#ifdef HAL_ENABLE_CHRONO_TIME
uint32_t chrono_time_millis() {
	return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
uint64_t chrono_time_highres() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
#endif
