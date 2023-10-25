#ifndef DART_HAL_WRAPPER_WINDOWS_WRAPPER_H_
#define DART_HAL_WRAPPER_WINDOWS_WRAPPER_H_

#include "../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Check if we compile for Windows
#ifdef USE_WINDOWS
	// Check if already any HAL defined
	#ifdef HAL_NAME
		#error "Already a HAL defined"
	#endif

	// Include any device specific stuff
	#include "../../../../chickenFlap/dart/hal_wrapper/windows/windows_includes.h"

	/**
	 * Defines the name for the HAL which is used.
	 */
	#define HAL_NAME "WINDOWS"

	#define HAL_ENABLE_CHRONO_TIME
	#define HAL_ENABLE_UART_STDIO

	#include "../../../../chickenFlap/dart/hal_wrapper/driver/drivers.h"

	#include "../../../../chickenFlap/dart/hal_wrapper/windows/highres_time.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/windows/filesystem.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/windows/network.h"
#endif

#endif
