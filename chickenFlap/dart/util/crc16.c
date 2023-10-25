#include "../../../chickenFlap/dart/util/crc16.h"

uint16_t crc16(uint8_t* data, size_t size) {
	return crc16OfPart(0, data, size);
}

uint16_t crc16_twoPart(uint8_t* data1, size_t size1, uint8_t* data2, size_t size2) {
	DART_NOT_NULL_RETURN(data1, DART_ERROR_INVALID_VALUE, 0);
	DART_NOT_NULL_RETURN(data2, DART_ERROR_INVALID_VALUE, 0);

	uint16_t crc = 0;
	crc = crc16OfPart(crc, data1, size1);
	crc = crc16OfPart(crc, data2, size2);
	return crc;
}

uint16_t crc16OfPart(uint16_t crc, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, 0);

	for (size_t i = 0; i < size; i++) {
	    crc  = (uint8_t)(crc >> 8) | (crc << 8);
	    crc ^= data[i];
	    crc ^= (uint8_t)(crc & 0xFF) >> 4;
	    crc ^= crc << 12;
	    crc ^= (crc & 0x00FF) << 5;
    }
	return crc;
}
