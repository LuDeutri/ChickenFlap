#include "../../../chickenFlap/dart/util/motorola.h"

// Intel
int8_t to_intel_int8(int8_t val) {
    return val;
}

uint8_t to_intel_uint8(uint8_t val) {
    return val;
}

uint16_t to_intel_uint16(uint16_t val) {
#ifdef IS_INTEL_DEVICE
    return val;
#else
    return (val << 8) | (val >> 8);
#endif
}

int16_t to_intel_int16(int16_t val) {
#ifdef IS_INTEL_DEVICE
    return val;
#else
    return (val << 8) | ((val >> 8) & 0xFF);
#endif
}

uint32_t to_intel_uint32(uint32_t val) {
#ifdef IS_INTEL_DEVICE
    return val;
#else
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
#endif
}

int32_t to_intel_int32(int32_t val) {
#ifdef IS_INTEL_DEVICE
    return val;
#else
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | ((val >> 16) & 0xFFFF);
#endif
}

uint64_t to_intel_uint64(uint64_t val) {
#ifdef IS_INTEL_DEVICE
    return val;
#else
    return  ((((val) >> 56) & 0x00000000000000FF) | (((val) >> 40) & 0x000000000000FF00) |
            (((val) >> 24) & 0x0000000000FF0000) | (((val) >>  8) & 0x00000000FF000000) |
            (((val) <<  8) & 0x000000FF00000000) | (((val) << 24) & 0x0000FF0000000000) |
            (((val) << 40) & 0x00FF000000000000) | (((val) << 56) & 0xFF00000000000000));
#endif
}

int8_t from_intel_int8(int8_t val) {
    return val;
}

uint8_t from_intel_uint8(uint8_t val) {
    return val;
}

uint16_t from_intel_uint16(uint16_t val) {
    return to_intel_uint16(val);
}

int16_t from_intel_int16(int16_t val) {
    return to_intel_int16(val);
}

uint32_t from_intel_uint32(uint32_t val) {
    return to_intel_uint32(val);
}

int32_t from_intel_int32(int32_t val) {
    return to_intel_int32(val);
}

uint64_t from_intel_uint64(uint64_t val) {
    return to_intel_uint64(val);
}

// Motorola

int8_t to_motorola_int8(int8_t val) {
	return val;
}

uint8_t to_motorola_uint8(uint8_t val) {
	return val;
}

uint16_t to_motorola_uint16(uint16_t val) {
#ifdef IS_MOTOROLA_DEVICE
    return val;
#else
    return (val << 8) | (val >> 8);
#endif
}

int16_t to_motorola_int16(int16_t val) {
#ifdef IS_MOTOROLA_DEVICE
    return val;
#else
    return (val << 8) | ((val >> 8) & 0xFF);
#endif
}

uint32_t to_motorola_uint32(uint32_t val) {
#ifdef IS_MOTOROLA_DEVICE
    return val;
#else
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
#endif
}

int32_t to_motorola_int32(int32_t val) {
#ifdef IS_MOTOROLA_DEVICE
    return val;
#else
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | ((val >> 16) & 0xFFFF);
#endif
}

int8_t from_motorola_int8(int8_t val) {
	return val;
}

uint8_t from_motorola_uint8(uint8_t val) {
	return val;
}

uint16_t from_motorola_uint16(uint16_t val) {
	return to_motorola_uint16(val);
}

int16_t from_motorola_int16(int16_t val) {
	return to_motorola_int16(val);
}

uint32_t from_motorola_uint32(uint32_t val) {
	return to_motorola_uint32(val);
}

int32_t from_motorola_int32(int32_t val) {
    return to_motorola_int32(val);
}
