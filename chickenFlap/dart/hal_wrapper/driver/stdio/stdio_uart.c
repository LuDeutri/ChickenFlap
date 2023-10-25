#include "../../../../../chickenFlap/dart/hal_wrapper/driver/stdio/stdio_uart.h"

#ifdef HAL_ENABLE_UART_STDIO

void hal_uart_init() {
    // Do nothing
}

void hal_uart_update() {
    // Do nothing
}

void hal_uart_transmit(int uart, uint8_t* data, size_t size) {
    if (uart != 0)
        return;

    fwrite(data, sizeof(uint8_t), size, stdout);
}

int hal_uart_receive(int uart, uint8_t* data, size_t size) {
    if (uart != 0)
        return 0;

    for (size_t i = 0; i < size; i++)
        data[i] = getchar();
    return size;
}

bool hal_uart_lin_sync_break(int uart) {
    DART_NOT_IMPLEMENTED_RETURN(false);
}

#endif
