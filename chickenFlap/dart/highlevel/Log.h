/**
 * Allows logging of text data.
 */
#pragma once

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/uart.h"
#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/time.h"

/**
 * The log level of the log messages.
 */
enum LogLevel {
	/**
	 * The log level Error. Should be used for serve errors.
	 */
	Error,
	/**
	 * The log level Error. Should be used for faults that occur but operation can continue.
	 */
	Warning,
	/**
	 * The log level Info. Should be used for informations that are important to the user.
	 */
	Info,
	/**
	 * The log level Debug. Should be used for informations that are important to the developer.
	 */
	Debug
};

/**
 * The maximum number of messages lines which are buffered in the log buffer.
 */
#define LOG_BUFFER_LINES 64

/**
 * Maximum length in bytes of a message.
 */
#define LOG_MAX_MESSAGE_LENGTH 128

class Log {
private:
	/**
	 * Is true when the Log module is enabled.
	 */
	static bool enabled;

	/**
	 * Internal index of the last message saved in _buffer (i.e. how many lines are saved in the buffer).
	 */
	static int bufferLines;
	/**
	 * The buffer which saves the log messages.
	 */
	static char** _buffer;

	/**
	 * Adds a message to the end of the buffer.
	 */
	static void pushMessage(char* str);

	/**
	 * Returns the string for the specified level.
	 */
	static const char* getLevelString(LogLevel level);

	/**
	 * Creates and formats a message with the specified log level, tag, format and the arguments used in the format.
	 */
	static void print(LogLevel level, const char* tag, const char* format, va_list args);
public:
	/**
	 * Enables or disables the Log module.
	 */
	static void setEnabled(bool enable);

	/**
	 * Adds an empty line to the log.
	 */
	static void emptyLine();

	/**
	 * Adds a message line formatted with the specified format and optional arguments. The used log level is Error.
	 */
	static void error(const char* tag, const char* format, ...);
	/**
	 * Adds a message line formatted with the specified format and optional arguments. The used log level is Warning.
	 */
	static void warning(const char* tag, const char* format, ...);
	/**
	 * Adds a message line formatted with the specified format and optional arguments. The used log level is Info.
	 */
	static void info(const char* tag, const char* format, ...);
	/**
	 * Adds a message line formatted with the specified format and optional arguments. The used log level is Debug.
	 */
	static void debug(const char* tag, const char* format, ...);

	/**
	 * Returns the number of lines saved in to the buffer. See bufferLines.
	 */
	static uint32_t getBufferLines();
	/**
	 * Returns the pointer to the internal buffer. See buffer.
	 */
	static char** getBuffer();
	/**
	 * Clears the buffer, i.e. removes every message saved into the buffer.
	 */
	static void clearBuffer();
	/**
	 * Removes the first message from the buffer and returns it. The message must be freed by the user.
	 */
	static char* popMessage();
};
