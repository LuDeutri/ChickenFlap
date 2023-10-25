#ifndef DART_HAL_WRAPPER_TMS320_ANALOG_H_
#define DART_HAL_WRAPPER_TMS320_ANALOG_H_

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef USE_TMS320

#include "../../../../chickenFlap/dart/analog.h"
#include "../../../../chickenFlap/dart/util/timer.h"

/**
 * Defines the value set for the ADC1 Group 1 FIFO buffer size in bytes.
 */
#define HAL_CONFIG_ADC1_GROUP1_FIFO_SIZE 16

void hal_adc_update_group(uint8_t group);

#endif

#endif
