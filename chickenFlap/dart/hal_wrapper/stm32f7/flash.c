#include "../../../../chickenFlap/dart/hal_wrapper/stm32f7/flash.h"

#if defined(USE_STM32F7) && defined(HAL_ENABLE_FLASH)

bool flash_read_data(void* pointer, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(pointer, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN((int)pointer % 4 == 0, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	// Flash memory is simply mapped in to the RAM :)
	memcpy(data, pointer, size);
	return true;
}

bool flash_write_data(uint32_t sector, void* pointer, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN((size_t)pointer % 4 == 0, DART_ERROR_INVALID_VALUE, false);

	size_t position = 0;
	size_t numberWords = (size / sizeof(uint32_t));

	// Unlock
	DART_ASSERT_RETURN(HAL_FLASH_Unlock() == HAL_OK, DART_ERROR_HAL, false);

	// Erase sector
	FLASH_EraseInitTypeDef erase;
	erase.TypeErase = TYPEERASE_SECTORS;
	erase.Sector = sector;
	erase.NbSectors = 1;
	erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	uint32_t sectorError;
	DART_ASSERT_RETURN(HAL_FLASHEx_Erase(&erase, &sectorError) == HAL_OK, DART_ERROR_HAL, false);

	// Write every word
	size_t wordsWritten = 0;
	while (wordsWritten < numberWords) {
		uint32_t* word = (uint32_t*)&data[position];
		DART_ASSERT_RETURN(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)((uint8_t*)pointer + position), *word) == HAL_OK, DART_ERROR_HAL, false);

		position += sizeof(uint32_t);
		wordsWritten++;
	}

	// Write last remaining bytes
	while (position < size) {
		DART_ASSERT_RETURN(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (uint32_t)((uint8_t*)pointer + position), data[position]) == HAL_OK, DART_ERROR_HAL, false);
		position += sizeof(uint8_t);
	}

	// Lock flash again
	DART_ASSERT_RETURN(HAL_FLASH_Lock() == HAL_OK, DART_ERROR_HAL, false);
	return true;
}

#endif
