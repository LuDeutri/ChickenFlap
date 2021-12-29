#ifndef DART_HAL_WRAPPER_LINUX_WRAPPER_H_
#define DART_HAL_WRAPPER_LINUX_WRAPPER_H_

#include "../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Check if we compile for Windows
#ifdef USE_LINUX
	// Check if already any HAL defined
	#ifdef HAL_NAME
		#error "Already a HAL defined"
	#endif

	// Include any device specific stuff
	#include "../../../../chickenFlap/dart/hal_wrapper/linux/linux_includes.h"

	/**
	 * Defines the name for the HAL which is used.
	 */
	#define HAL_NAME "LINUX"

	#define HAL_ENABLE_CHRONO_TIME
	#define HAL_ENABLE_UART_STDIO

	#include "../../../../chickenFlap/dart/hal_wrapper/driver/drivers.h"

	#include "../../../../chickenFlap/dart/hal_wrapper/linux/highres_time.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/linux/filesystem.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/linux/network.h"
#endif

#endif
