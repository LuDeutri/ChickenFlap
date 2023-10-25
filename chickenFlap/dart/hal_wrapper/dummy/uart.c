#include "../../../../chickenFlap/dart/hal_wrapper/dummy/uart.h"

#ifdef HAL_DUMMY_UART

void hal_uart_init() {
	// Do nothing
}
void hal_uart_update() {
	// Do nothing
}

void hal_uart_transmit(int uart, uint8_t* data, size_t size) {
	// Do nothing
}

bool hal_uart_lin_sync_break(int uart) {
	// Do nothing
	return false;
}
#endif



