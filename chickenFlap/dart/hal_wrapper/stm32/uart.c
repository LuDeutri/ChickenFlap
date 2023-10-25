#include "../../../../chickenFlap/dart/hal_wrapper/stm32/uart.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_UART)
UART_HandleTypeDef* uartMap[UART_COUNT] = UART_MAP;
uint8_t uartReadBuffer[UART_COUNT];

void hal_uart_init() {
	// HAL init done in main.c

	// Start receiving
	for (int i = 0; i < UART_COUNT; i++)
		if (uartMap[i] != NULL)
			DART_ASSERT_HAL_OK(HAL_UART_Receive_IT(uartMap[i], &uartReadBuffer[i], 1));
}
void hal_uart_update() {
	// Do nothing
}

void hal_uart_transmit(int uart, uint8_t* data, size_t size) {
	DART_NOT_NULL(uartMap[uart], DART_ERROR_INVALID_VALUE);

	DART_ASSERT_HAL_OK(HAL_UART_Transmit(uartMap[uart], data, size, 50));
}

bool hal_uart_lin_sync_break(int uart) {
	DART_NOT_NULL_RETURN(uartMap[uart], DART_ERROR_INVALID_VALUE, false);

	return HAL_LIN_SendBreak(uartMap[uart]) == HAL_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	for (int i = 0; i < UART_COUNT; i++) {
		if (uartMap[i] == huart) {
			_dartUART_fill_receive_buffer(i, &uartReadBuffer[i], 1);
			HAL_UART_Receive_IT(huart, &uartReadBuffer[i], 1);
		}
	}
}
#endif
