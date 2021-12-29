#include "../../../chickenFlap/dart/util/crc32.h"

uint32_t crc32(uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, 0);

	uint32_t crc = 0xFFFFFFFF;
	crc32OfPart(crc, data, size);
	return crc ^ 0xFFFFFFFF;
}

uint32_t crc32_twoPart(uint8_t* data1, size_t size1, uint8_t* data2, size_t size2) {
	DART_NOT_NULL_RETURN(data1, DART_ERROR_INVALID_VALUE, 0);
	DART_NOT_NULL_RETURN(data2, DART_ERROR_INVALID_VALUE, 0);

	uint32_t crc = 0xFFFFFFFF;
	crc = crc32OfPart(crc, data1, size1);
	crc = crc32OfPart(crc, data2, size2);
	return crc ^ 0xFFFFFFFF;
}

uint32_t crc32OfPart(uint32_t crc, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, 0);

	const uint32_t polynom = 0xEDB88320;
	for (size_t i = 0; i < size; i++) {
		uint8_t b = data[i];
		for (int j = 0; j < 8; j++) {
			if ((crc & 1) != (b & 1))
				crc = (crc >> 1) ^ polynom;
			else
				crc >>= 1;
			b >>= 1;
		}
	}
	return crc;
}
