#include "../../../chickenFlap/dart/util/crc8.h"

uint8_t crc8(uint8_t *data, size_t len, uint8_t key) {
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, 0);

	if (len == 0)
		return 0;

	uint8_t crc = 0;
	while (len-- > 0) {
		for (uint8_t i = 0x80; i != 0; i /= 2) {
			if ((crc & 0x80) != 0) {
				crc *= 2;
				crc ^= key;
			} else
				crc *= 2;

			if ((*data & i) != 0)
				crc ^= key;
		}
		data++;
	}
	return crc;
}
