#ifndef DART_UTIL_MOTOROLA_H_
#define DART_UTIL_MOTOROLA_H_

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__LITTLE_ENDIAN__) && (defined(__IEEE_BIG_ENDIAN) || __BIG_ENDIAN__)
    #define IS_MOTOROLA_DEVICE 1
    #define IS_BIG_ENDIAN
#else
    #define IS_INTEL_DEVICE 1
    #define IS_LITTLE_ENDIAN
#endif

int8_t to_intel_int8(int8_t val);
uint8_t to_intel_uint8(uint8_t val);
uint16_t to_intel_uint16(uint16_t val);
int16_t to_intel_int16(int16_t val);
uint32_t to_intel_uint32(uint32_t val);
int32_t to_intel_int32(int32_t val);
uint64_t to_intel_uint64(uint64_t val);


int8_t from_intel_int8(int8_t val);
uint8_t from_intel_uint8(uint8_t val);
uint16_t from_intel_uint16(uint16_t val);
int16_t from_intel_int16(int16_t val);
uint32_t from_intel_uint32(uint32_t val);
int32_t from_intel_int32(int32_t val);
uint64_t from_intel_uint64(uint64_t val);

int8_t to_motorola_int8(int8_t val);
uint8_t to_motorola_uint8(uint8_t val);
uint16_t to_motorola_uint16(uint16_t val);
int16_t to_motorola_int16(int16_t val);
uint32_t to_motorola_uint32(uint32_t val);
int32_t to_motorola_int32(int32_t val);

int8_t from_motorola_int8(int8_t val);
uint8_t from_motorola_uint8(uint8_t val);
uint16_t from_motorola_uint16(uint16_t val);
int16_t from_motorola_int16(int16_t val);
uint32_t from_motorola_uint32(uint32_t val);
int32_t from_motorola_int32(int32_t val);

#ifdef __cplusplus
}
#endif

#endif
