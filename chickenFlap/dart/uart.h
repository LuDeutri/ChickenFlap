/**
 * Wrapper around the UART  HAL. Implements functions for initializing the UART, receiving and sending data.
 */
#ifndef DART_UART_H_
#define DART_UART_H_


#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maximum size of the receive buffer in bytes.
 */
#ifndef UART_RECEIVE_BUFFER_SIZE
#define UART_RECEIVE_BUFFER_SIZE 96
#endif

#ifndef UART_ASSERT_ON_RECEIVE_BUFFER_OVERFLOW
#define UART_ASSERT_ON_RECEIVE_BUFFER_OVERFLOW 1
#endif

typedef struct {
	uint8_t data[UART_RECEIVE_BUFFER_SIZE];
	size_t index;
} uart_receive_buffer_t;

#ifndef UART_NEW_LINE_LENGTH
/**
 * Length of UART_NEW_LINE.
 * When UART_NEW_LINE is changed UART_NEW_LINE_LENGTH must be changed too.
 */
#define UART_NEW_LINE_LENGTH 2
#endif

#ifndef UART_NEW_LINE
/**
 * The char which indicates a new line for the dartUART_writeLine and dartUART_readLine functions.
 * When UART_NEW_LINE is changed UART_NEW_LINE_LENGTH must be changed too.
 */
#define UART_NEW_LINE { '\r', '\n' }
#endif

/**
 * Initializes the UART HAL.
 * MUST be called before any other UART function.
 */
void dartUART_init();

/**
 * Updates the UART HAL.
 */
void dartUART_update();

void dartUART_transmit_byte(int uart, uint8_t data);

/**
 * Transmits the specified data with the specified size over the given UART.
 */
void dartUART_transmit(int uart, uint8_t* data, size_t size);

/**
 * Writes the specified string to the given UART.
 */
void dartUART_write(int uart, const char* str);

/**
 * Returns the number of bytes available in the receive buffer.
 */
size_t dartUART_readBytesAvailable(int uart);

/**
 * Tries to receive the specified data. Returns the count of bytes read.
 */
size_t dartUART_read(int uart, uint8_t* data, size_t size);

/**
 * Writes one line to the given UART. The string is transmitted over the UART and then followed by the new line.
 */
void dartUART_writeLine(int uart, const char* str);

/**
 * Writes one formatted string to the given UART. The string is transmitted over the UART.
 */
void dartUART_format(int uart, const char* format, ...);

/**
 * Writes one formatted line to the given UART. The string is transmitted over the UART and then followed by the new line.
 */
void dartUART_formatLine(int uart, const char* format, ...);

/**
 * Reads one line from the given UART. Data is read until the new line and copied into the str buffer.
 * Returns true when one complete line could be and false otherwise. When no line could be read the receive buffer is not cleared.
 */
bool dartUART_readLine(int uart, char* str, size_t size);

/**
 * Puts data into the internal receive buffer.
 */
void _dartUART_fill_receive_buffer(int uart, uint8_t* data, size_t size);

/**
 * Sends the LIN sync break signal to the UART.
 */
bool dartUART_lin_syncBreak(int uart);

/**
 * Initializes the HAL behind the UART. Implemented by the HAL wrapper.
 */
void hal_uart_init();
/**
 * Updates the HAL behind the UART. Implemented by the HAL wrapper.
 */
void hal_uart_update();
/**
 * Calls the HAL to transmit the given data over the given UART. Implemented by the HAL wrapper.
 */
void hal_uart_transmit(int uart, uint8_t* data, size_t size);

/**
 * Sends the LIN sync break signal to the UART. Implemented by the HAL wrapper.
 */
bool hal_uart_lin_sync_break(int uart);

#ifdef __cplusplus
}
#endif

#endif
