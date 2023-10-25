#ifndef DART_HAL_WRAPPER_HAL_WRAPPER_H_
#define DART_HAL_WRAPPER_HAL_WRAPPER_H_

#include "../../../chickenFlap/dart/standard_include.h"

// Functions
#ifdef __cplusplus
extern "C" {
#endif
	/**
	* Initializes the HAL.
	*/
	void hal_init();

	/**
	* Yields to any other thread running.
	*/
	void hal_yield();

	/**
	 * Resets the system.
	 */
	void hal_system_reset();

	/**
	* Returns the free heap in bytes.
	*/
	size_t hal_get_free_heap();
#ifdef __cplusplus
}
#endif


// Include boards
#include "../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Based on the board the correct HAL wrapper is included
#include "../../../chickenFlap/dart/hal_wrapper/dummy/dummy_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/atmega32m1/atmega32m1_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/stm32f1/stm32f1_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/stm32f7/stm32f7_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/tms320/tms320_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/tms570/tms570_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/windows/windows_wrapper.h"
#include "../../../chickenFlap/dart/hal_wrapper/linux/linux_wrapper.h"

// Include any drivers
#include "../../../chickenFlap/dart/hal_wrapper/driver/drivers.h"

// Check for C++ and for C, use different error messages
#ifdef __cplusplus
// Check if any HAL was defined
	#if !defined(HAL_NAME)
		#error "No HAL wrapper defined (C++ settings), check if you have an USE_BOARD_* define!"
	#endif

	// Check if any Board was defined
	#if !defined(HAL_BOARD)
		#error "No board defined (C++ settings), check if you have an USE_BOARD_* define!"
	#endif
#else
	// Check if any HAL was defined
	#if !defined(HAL_NAME)
		#error "No HAL wrapper defined (C settings), check if you have an USE_BOARD_* define!"
	#endif

	// Check if any Board was defined
	#if !defined(HAL_BOARD)
		#error "No board defined (C settings), check if you have an USE_BOARD_* define!"
	#endif
#endif


#endif
