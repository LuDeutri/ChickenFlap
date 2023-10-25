#ifndef SRC_UTIL_ERROR_H_
#define SRC_UTIL_ERROR_H_

#include "../../../chickenFlap/dart/util/types.h"

#define DART_ERROR_UNKNOWN 0x00
#define DART_ERROR_INTERNAL 0x01
#define DART_ERROR_INVALID_VALUE 0x02
#define DART_ERROR_HAL 0x03
#define DART_ERROR_UTIL 0x04
#define DART_ERROR_BUFFER_OVERFLOW 0x05
#define DART_ERROR_ID_ALREADY_IN_USE 0x06
#define DART_ERROR_OUT_OF_MEMORY 0x07
#define DART_ERROR_NOT_IMPLEMENTED 0x08
#define DART_ERROR_TIMEOUT 0x09
#define DART_ERROR_VALUE_IS_NULL 0x0A


#ifdef __cplusplus
extern "C" {
#endif

/**
* Function that only calls the error function, but does NOT return.
*/
#define DART_SIGNAL_ERROR(code) do { dart_error_wrapper((code), __FILE__, __LINE__); } while (0)

/**
 * Calls error and ends (return void) the calling function when a is not true.
 */
#define DART_ASSERT(a, code) do { if (unlikely(!(a))) { DART_SIGNAL_ERROR(code); return; } } while(0)

/**
 * Calls error and ends the calling function with the return value ret when a is not true.
 * See DART_ASSERT().
 */
#define DART_ASSERT_RETURN(a, code, ret) do { if (unlikely(!(a))) { DART_SIGNAL_ERROR(code); return (ret); } } while(0)

/**
 * Calls error and ends (return void) the calling function when a is NULL.
 */
#define DART_NOT_NULL(a, code) DART_ASSERT((void*)(a) > (void*)1024, DART_ERROR_VALUE_IS_NULL)

/**
 * Calls error and ends the calling function with the return value ret when a is NULL.
 * See NOT_NULL().
 */
#define DART_NOT_NULL_RETURN(a, code, ret) DART_ASSERT_RETURN((void*)(a) > (void*)1024, DART_ERROR_VALUE_IS_NULL, (ret))

/**
 * Function that can be used to show that some part of the code is not implemented.
 */
#define DART_NOT_IMPLEMENTED() DART_ASSERT(false, DART_ERROR_NOT_IMPLEMENTED)

/**
 * Function that can be used to show that some part of the code is not implemented.
 */
#define DART_NOT_IMPLEMENTED_RETURN(ret) DART_ASSERT_RETURN(false, DART_ERROR_NOT_IMPLEMENTED, ret)

/**
 * Function that can be used to call the error function and return nothing.
 */
#define DART_ERROR(code) DART_ASSERT(false, code)

/**
 * Function that can be used to call the error function and return a given value.
 */
#define DART_ERROR_RETURN(code, ret) DART_ASSERT_RETURN(false, code, ret)

/**
 * Marco that can be used to indicate that a parameter is unused in the method. This will prevent the warning.
 */
#define DART_NOT_USED(x) ((void)x)

/**
 * Returns the name of the error code as string.
 */
const char* DART_ERROR_STR(int errorCode);

/**
 * Wrapper which is used for the error marcos. Will either call the new dart error or the old.
 * @param errorCode The code of the error.
 * @param file The file where the assert was called.
 * @param line The line where the assert was called.
 */
void dart_error_wrapper(int errorCode, const char* file, int line);

/**
 * Defines global error function. Called when any error occurs.
 * MUST be implemented by the user.
 */
#ifdef USE_NEW_DART_ERROR
    void dart_error(int errorCode, const char* file, int line);
#else
    //void dart_error(int errorCode);
#endif

#ifdef __cplusplus
}
#endif

#endif
