#ifndef DART_HAL_WRAPPER_STM32_DAC_H_
#define DART_HAL_WRAPPER_STM32_DAC_H_

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_DAC)
#define DAC_MAX_VALUE 4095
#include "../../../../chickenFlap/dart/dac.h"
#endif


#endif
