#include "../../../../chickenFlap/dart/hal_wrapper/stm32f1/spi.h"

#if defined(USE_STM32F1) && defined(HAL_ENABLE_SPI)
bool hal_spi_set_speed(int spi, int frequency) {
	// Not implemented
	return true;
}
#endif
