#include "../../../../chickenFlap/dart/hal_wrapper/dummy/flash.h"

#ifdef HAL_DUMMY_FLASH
bool flash_read_data(void* pointer, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(pointer, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	memset(data, 0, size);
	return false;
}

bool flash_write_data(uint32_t sector, void* pointer,  uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(pointer, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	return false;
}
#endif
