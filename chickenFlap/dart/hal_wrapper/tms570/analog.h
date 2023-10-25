#ifndef DART_HAL_WRAPPER_TMS570_ANALOG_H_
#define DART_HAL_WRAPPER_TMS570_ANALOG_H_

#include "../../../../chickenFlap/dart/analog.h"

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../../chickenFlap/dart/util/timer.h"

#if defined(USE_TMS570) && defined(HAL_ENABLE_ADC)
/**
 * Defines the value set for the ADC1 Group 1 FIFO buffer size in bytes.
 */
#define HAL_CONFIG_ADC1_GROUP1_FIFO_SIZE 16

void hal_adc_update_group(uint8_t group);
#endif

#endif
