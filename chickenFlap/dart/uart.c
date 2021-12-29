#include "../../chickenFlap/dart/uart.h"

uart_receive_buffer_t uartReceiveBuffer[UART_COUNT];

void dartUART_init() {
	memset(&uartReceiveBuffer, 0, sizeof(uartReceiveBuffer));

	hal_uart_init();
}

void dartUART_update() {
	hal_uart_update();
}

void dartUART_transmit_byte(int uart, uint8_t data) {
	dartUART_transmit(uart, &data, 1);
}

void dartUART_transmit(int uart, uint8_t* data, size_t size) {
	DART_ASSERT(uart >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(uart < UART_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);

	if (size > 0)
		hal_uart_transmit(uart, data, size);
}

size_t dartUART_readBytesAvailable(int uart) {
	DART_ASSERT_RETURN(uart >= 0, DART_ERROR_INVALID_VALUE, 0);
	DART_ASSERT_RETURN(uart < UART_COUNT, DART_ERROR_INVALID_VALUE, 0);
	return uartReceiveBuffer[uart].index;
}

size_t dartUART_read(int uart, uint8_t* data, size_t size) {
	DART_ASSERT_RETURN(uart >= 0, DART_ERROR_INVALID_VALUE, 0);
	DART_ASSERT_RETURN(uart < UART_COUNT, DART_ERROR_INVALID_VALUE, 0);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, 0);

	size_t dataToRead = min(size, uartReceiveBuffer[uart].index);
	memcpy(data, uartReceiveBuffer[uart].data, dataToRead);
	memmove(uartReceiveBuffer[uart].data, uartReceiveBuffer[uart].data + dataToRead, uartReceiveBuffer[uart].index - dataToRead);
	uartReceiveBuffer[uart].index -= dataToRead;
	return dataToRead;
}


void dartUART_write(int uart, const char* str) {
	DART_NOT_NULL(str, DART_ERROR_INVALID_VALUE);

	dartUART_transmit(uart, (uint8_t*)str, strlen(str));
}

void dartUART_writeLine(int uart, const char* str) {
	DART_NOT_NULL(str, DART_ERROR_INVALID_VALUE);

	uint8_t newLine[UART_NEW_LINE_LENGTH] = UART_NEW_LINE;
	dartUART_transmit(uart, (uint8_t*)str, strlen(str));
	dartUART_transmit(uart, newLine, sizeof(newLine));
}

void dartUART_format(int uart, const char* format, ...) {
	DART_NOT_NULL(format, DART_ERROR_INVALID_VALUE);

	va_list args;
	va_start(args, format);

	char buffer[64];
	int size = vsnprintf(buffer, sizeof(buffer), format, args);
	if (size >= 0)
		dartUART_write(uart, buffer);
	va_end(args);
}

void dartUART_formatLine(int uart, const char* format, ...) {
	DART_NOT_NULL(format, DART_ERROR_INVALID_VALUE);

	va_list args;
	va_start(args, format);

	char buffer[128];
	int size = vsnprintf(buffer, sizeof(buffer), format, args);
	if (size >= 0)
		dartUART_writeLine(uart, buffer);
	va_end(args);
}

bool dartUART_readLine(int uart, char* str, size_t size) {
	DART_ASSERT_RETURN(uart >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(uart < UART_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(str, DART_ERROR_INVALID_VALUE, false);

	// Clear string
	memset(str, 0, size);

	// Check for line ending
	size_t bufferSize = uartReceiveBuffer[uart].index;
	for (size_t i = 0; i < bufferSize; i++) {
		if (uartReceiveBuffer[uart].data[i] == '\r' || uartReceiveBuffer[uart].data[i] == '\n') {
			size_t length;
			size_t lineFeedLength;

			// \r\n logic
			if (uartReceiveBuffer[uart].data[i] == '\r' && i + 1 < bufferSize && uartReceiveBuffer[uart].data[i + 1] == '\n')
			{
				length = i;
				lineFeedLength = 2;
			}
			else
			{
				length = i;
				lineFeedLength = 1;
			}

			// Ensure string fits into buffer + ending zero
			DART_ASSERT_RETURN(size + 1 >= length, DART_ERROR_BUFFER_OVERFLOW, false);
			dartUART_read(uart, (uint8_t*)str, length);

			// Read line feed
			uint8_t dummy[2];
			dartUART_read(uart, dummy, lineFeedLength);
			return true;
		}
	}
	return false;
}

void _dartUART_fill_receive_buffer(int uart, uint8_t* data, size_t size) {
	#if UART_ASSERT_ON_RECEIVE_BUFFER_OVERFLOW
		DART_ASSERT(uartReceiveBuffer[uart].index + size <= UART_RECEIVE_BUFFER_SIZE, DART_ERROR_BUFFER_OVERFLOW);
	#else
		if (uartReceiveBuffer[uart].index + size > UART_RECEIVE_BUFFER_SIZE)
			return;
	#endif

	memcpy(uartReceiveBuffer[uart].data + uartReceiveBuffer[uart].index, data, size);
	uartReceiveBuffer[uart].index += size;
}

bool dartUART_lin_syncBreak(int uart) {
	DART_ASSERT_RETURN(uart >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(uart < UART_COUNT, DART_ERROR_INVALID_VALUE, false);

	return hal_uart_lin_sync_break(uart);
}
