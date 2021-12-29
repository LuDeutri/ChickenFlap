#ifndef DART_HAL_WRAPPER_STM32_ERROR_H_
#define DART_HAL_WRAPPER_STM32_ERROR_H_

/**
 * Calls error and ends (return void) when the return value of a is not HAL_OK.
 */
#define DART_ASSERT_HAL_OK(a) DART_ASSERT((a) == HAL_OK, DART_ERROR_HAL)

/**
 * Calls error and ends (return void) when the return value of a is not HAL_OK.
 */
#define DART_ASSERT_HAL_OK_RETURN(a, b) DART_ASSERT_RETURN((a) == HAL_OK, DART_ERROR_HAL, (b));



#endif
