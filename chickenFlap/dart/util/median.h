#ifndef SRC_UTIL_MEDIAN_H_
#define SRC_UTIL_MEDIAN_H_

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/util/bubbleSort.h"
#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Finds the median of the specified array with the specified length. The specified array
 * is modified, because the array is sorted to find the median value of the array.
 * length MUST be larger than 0.
 */
uint16_t median(uint16_t* array, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
