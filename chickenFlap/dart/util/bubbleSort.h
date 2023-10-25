#ifndef SRC_UTIL_BUBBLESORT_H_
#define SRC_UTIL_BUBBLESORT_H_

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sorts the specified array with the specified length by using the bubble sort algorithm.
 * When length is 0 no sorting is done.
 */
void sortUint16(uint16_t* array, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
