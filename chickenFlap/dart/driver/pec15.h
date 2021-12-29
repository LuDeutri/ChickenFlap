#ifndef DART_DRIVER_PEC15_H_
#define DART_DRIVER_PEC15_H_

#include "../../../chickenFlap/dart/standard_include.h"
#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Calculates the PEC15 checksum for the given data with the given size in bytes.
 * The PEC15 is shifted by one bit to the left, to produce a 16 bit value.
 */
uint16_t pec15_calc(uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif
