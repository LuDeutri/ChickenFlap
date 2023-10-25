#ifndef DART_UTIL_CRC32_H_
#define DART_UTIL_CRC32_H_

#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t crc32(uint8_t* data, size_t size);
uint32_t crc32_twoPart(uint8_t* data1, size_t size1, uint8_t* data2, size_t size2);
uint32_t crc32OfPart(uint32_t crc, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif
