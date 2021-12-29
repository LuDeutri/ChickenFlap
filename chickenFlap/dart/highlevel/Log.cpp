#include "../../../chickenFlap/dart/highlevel/Log.h"

bool Log::enabled = false;
int Log::bufferLines = 0;
char** Log::_buffer;

void Log::setEnabled(bool enable) {
	Log::enabled = enable;
}

const char* Log::getLevelString(LogLevel level) {
	// all strings should have the same length
	switch (level) {
	case Error:
		return "[Error]";
	case Warning:
		return " [Warn]";
	case Info:
		return " [Info]";
	case Debug:
		return "[Debug]";
	}
	return     "[Unkwn]";
}

uint32_t Log::getBufferLines() {
	return bufferLines;
}

char** Log::getBuffer() {
#if LOG_BUFFER_LINES > 0
	if (_buffer == NULL)
		_buffer = new char*[LOG_BUFFER_LINES];
	return _buffer;
#else
	return NULL;
#endif
}

void Log::clearBuffer() {
#if LOG_BUFFER_LINES > 0
	char** buffer = getBuffer();
	DART_NOT_NULL(buffer, DART_ERROR_INVALID_VALUE);

	for (int i = 0; i < bufferLines; i++) {
		delete buffer[i];
		buffer[i] = NULL;
	}
	bufferLines = 0;
#endif
}

char* Log::popMessage() {
#if LOG_BUFFER_LINES > 0
	if (!Log::enabled)
		return NULL;
	if (bufferLines <= 0)
		return NULL;

	char** buffer = getBuffer();
	DART_NOT_NULL_RETURN(buffer, DART_ERROR_INVALID_VALUE, NULL);

	char* msg = buffer[0]; // save first message

	// Shift every message up
	for (int i = 1; i < bufferLines; i++)
		buffer[i - 1] = buffer[i];

	bufferLines--;
	return msg;
#else
	return NULL;
#endif
}

void Log::pushMessage(char* message) {
	DART_NOT_NULL(message, DART_ERROR_INVALID_VALUE);
#ifdef UART_LOG
	dartUART_writeLine(UART_LOG, message);
#endif

#if LOG_BUFFER_LINES > 0
	char** buffer = getBuffer();
	DART_NOT_NULL(buffer, DART_ERROR_INVALID_VALUE);

	// Buffer is full
	if (bufferLines >= LOG_BUFFER_LINES)
		delete popMessage(); // remove oldest message

	// save message
	DART_ASSERT(bufferLines < LOG_BUFFER_LINES, DART_ERROR_INTERNAL);
	buffer[bufferLines++] = message;
#else
	delete message;
#endif
}

void Log::emptyLine() {
	if (!Log::enabled)
		return;

	char* message = new char[1];

	// Create message
	if (message != NULL) {
		message[0] = '\0';
		pushMessage(message);
	}
}

void Log::print(LogLevel level, const char* tag, const char* format, va_list args) {
	DART_NOT_NULL(tag, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(format, DART_ERROR_INVALID_VALUE);
	if (!Log::enabled)
		return;

	size_t messageSize = LOG_MAX_MESSAGE_LENGTH * sizeof(char);
	char* message = new char[messageSize];

	if (message == NULL) {
		// malloc() failed
        DART_ERROR(DART_ERROR_OUT_OF_MEMORY);
	}

	memset(message, 0, messageSize);

	// Format the start of the message (i.e. time and tag)
	int size = snprintf(message, messageSize, "$ [%8lus] %s [%14s]  ", (long unsigned int)(millis() / 1000), getLevelString(level), tag);
	if (size < 0) {
		// Could not log message, because message is too long
		delete[] message;
        DART_ERROR(DART_ERROR_UTIL);
	}

	// Format the message and add it to the end of the time and tag
	size = vsnprintf(message + size, messageSize - size, format, args);
	if (size < 0) {
		// Could not log message, because message is too long
		delete[] message;
        DART_ERROR(DART_ERROR_UTIL);
	}

	pushMessage(message);
}

void Log::error(const char* tag, const char* format, ...) {
	if (!Log::enabled)
			return;
	va_list args;
	va_start(args, format);
	print(Error, tag, format, args);
	va_end(args);
}

void Log::warning(const char* tag, const char* format, ...) {
	if (!Log::enabled)
			return;
	va_list args;
	va_start(args, format);
	print(Warning, tag, format, args);
	va_end(args);
}

void Log::info(const char* tag, const char* format, ...) {
	if (!Log::enabled)
			return;
	va_list args;
	va_start(args, format);
	print(Info, tag, format, args);
	va_end(args);
}

void Log::debug(const char* tag, const char* format, ...) {
	if (!Log::enabled)
			return;
	va_list args;
	va_start(args, format);
	print(Debug, tag, format, args);
	va_end(args);
}
