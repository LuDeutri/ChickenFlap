#ifndef DART_HAL_WRAPPER_DUMMY_DAC_H_
#define DART_HAL_WRAPPER_DUMMY_DAC_H_



#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef HAL_DUMMY_GPIO
#include "../../../../chickenFlap/dart/dac.h"

#define DAC_MAX_VALUE 4095
#endif
#endif
