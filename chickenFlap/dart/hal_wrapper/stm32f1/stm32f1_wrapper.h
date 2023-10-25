#ifndef DART_HAL_WRAPPER_STM32F1_WRAPPER_H_
#define DART_HAL_WRAPPER_STM32F1_WRAPPER_H_

#include "../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Check if we compile for the STM32F1 hardware
#ifdef USE_STM32F1
	// Check if already any HAL defined
	#ifdef HAL_NAME
		#error "Already a HAL defined"
	#endif

	// Include any device specific stuff
	#include <stm32f1xx_hal.h>

	#ifdef HAL_ENABLE_FREERTOS
		#include <FreeRTOS.h>
	#endif

	/**
	 * Defines the name for the HAL which is used.
	 */
	#define HAL_NAME "STM32F1"

	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/analog.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/can.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/digital.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/uart.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/highres_time.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/spi.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/error.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/dac.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/pwm.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/realTimeClock.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/i2c.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32/error.h"

    #include "../../../../chickenFlap/dart/hal_wrapper/stm32f1/can.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/stm32f1/spi.h"
#endif

#endif
