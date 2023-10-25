#ifndef DART_FLASH_H_
#define DART_FLASH_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads data from the internal flash.
 */
bool flash_read_data(void* pointer, uint8_t* data, size_t size);

/**
 * Writes data to the internal flash.
 */
bool flash_write_data(uint32_t sector, void* pointer, uint8_t* data, size_t size);


#ifdef __cplusplus
}
#endif

#endif
