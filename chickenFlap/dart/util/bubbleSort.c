#include "../../../chickenFlap/dart/util/bubbleSort.h"

void sortUint16(uint16_t* array, uint32_t length) {
	DART_NOT_NULL(array, DART_ERROR_UTIL);

    // Empty array? no sorting required
    // Ensuring (length - 1) does not overflow
    if (length == 0)
        return;

    bool sorted;
    do {
        // Reset sorted flag
        sorted = true;
        for (uint32_t i = 0; i < length - 1; i++) {
            // Get a and b
            uint16_t a = array[i];
            uint16_t b = array[i + 1];

            // Not sorted
            if (a > b) {
                // Swap a and b
                array[i] = b;
                array[i + 1] = a;

                sorted = false;
            }
        }
    }
    while (!sorted); // Sort until array is sorted
}
