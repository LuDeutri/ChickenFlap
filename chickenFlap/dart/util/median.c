#include "../../../chickenFlap/dart/util/median.h"

uint16_t median(uint16_t* array, uint32_t length) {
    DART_NOT_NULL_RETURN(array, DART_ERROR_UTIL, 0);
    DART_ASSERT_RETURN(length > 0, DART_ERROR_UTIL, 0);

    // Only one element in array
    if (length == 1)
        return array[0];

    // To find the median we first have to sort the array
    sortUint16(array, length);

    uint32_t mid = length / 2;
    if (length % 2 == 0) // There is no real "middle"
        return (array[mid] + array[mid - 1]) / 2; // Average of the elements in the middle
    return array[mid];
}
