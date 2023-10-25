#ifndef DART_HAL_WRAPPER_STM32_REALTIMECLOCK_H_
#define DART_HAL_WRAPPER_STM32_REALTIMECLOCK_H_

#include "../../../../chickenFlap/dart/realTimeClock.h"

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_RTC)

#define STM32_RTC_YEAR_OFFSET 2000

#endif

#endif
