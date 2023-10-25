#ifndef DART_UTIL_CRC16_H_
#define DART_UTIL_CRC16_H_

#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t crc16(uint8_t* data, size_t size);
uint16_t crc16_twoPart(uint8_t* data1, size_t size1, uint8_t* data2, size_t size2);
uint16_t crc16OfPart(uint16_t crc, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif
