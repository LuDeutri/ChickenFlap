#ifndef DART_UTIL_CRC8_H_
#define DART_UTIL_CRC8_H_

#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t crc8(uint8_t *data, size_t len, uint8_t key);

#ifdef __cplusplus
}
#endif


#endif
