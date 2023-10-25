#pragma once

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/highlevel/Log.h"
#include "../../../chickenFlap/dart/highlevel/PacketBuffer.h"
#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/time.h"

/**
 * Maximum number of functions that can be profiled.
 */
#define PROFILE_SIZE 48

struct ProfilerFunction {
	uint32_t index;
	char* name;
	uint64_t lastTime;
	uint64_t time;
	uint64_t maxTime;

	bool shouldReset;
	uint64_t currentTime;
};

class Profiler
{
private:
	static ProfilerFunction* functions;
	static uint32_t length;

	static uint32_t* stack;
	static uint32_t stackCurrent;

	static long lastMaxReset;

	static ProfilerFunction* findFunction(const char* name);
	static uint32_t start(const char* name, bool reset);
	static uint64_t stop(ProfilerFunction* function, bool add);

public:
	static bool enabled;

	/**
	 * Initializes the profiler module.
	 */
	static void init();

	/**
	 * Starts the time keeping on a function with the given name.
	 * Returns the index of the function.
	 */
	static uint32_t start(const char* name);
	/**
	 * Stops the time keeping on a function with the given name.
	 * Returns the measured time in microseconds.
	 */
	static uint64_t stop(const char* name);

	/**
	 * Starts the time keeping of a function with the given name and saves the function name to the stack.
	 */
	static void begin(const char* name);
	/**
	 * Stops the time keeping of the function that was last pushed to the stack and removes it from the stack.
	 * Returns the measured time in microseconds.
	 */
	static uint64_t end();

	/**
	 * See end(). Returns the value given in returnValue.
	 */
	static bool end(bool returnValue);

	static void restart(const char* name);

	/**
	 * Stops the time keeping of any function still left on the stack and prints an error when the stack is not empty.
	 */
	static void finishFrame();

	static void setTime(const char* name, uint32_t value);
	static void setTimeMillis(const char* name, uint32_t value);

#ifdef PACKETBUFFER_ENABLED
	/**
	 * Writes the data that was profiled to the packet buffer in a special format.
	 */
	static void writeData(PacketBuffer* buffer);
#endif
};
