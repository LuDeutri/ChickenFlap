#pragma once

#include "../../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
#ifdef HAL_ENABLE_CHRONO_TIME

// Fix chrono include (hacky workaround)
#undef min
#undef max

#include <chrono>

// Include math again (hacky workaround)
#undef SRC_UTIL_MATH_H_
#include "../../../../../chickenFlap/dart/util/math.h"

extern "C" {
	uint32_t chrono_time_millis();
	uint64_t chrono_time_highres();
}

#endif
#endif
