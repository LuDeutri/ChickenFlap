#include "../../../chickenFlap/dart/util/hash.h"

uint16_t hash_uint16(uint16_t a) {
	// Naive approach, may not work that good
	uint32_t h = hash_uint32(a << 16 | a);
	return (uint16_t)((h >> 16) ^ (h & 0xFFFF));
}

uint32_t hash_uint32(uint32_t a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}
