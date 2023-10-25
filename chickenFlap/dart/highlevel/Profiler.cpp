#include "../../../chickenFlap/dart/highlevel/Profiler.h"

bool Profiler::enabled = true;

ProfilerFunction* Profiler::functions = NULL;
uint32_t Profiler::length = 0;

uint32_t* Profiler::stack = NULL;
uint32_t Profiler::stackCurrent = 0;

long Profiler::lastMaxReset = 0;

void Profiler::init() {
	functions = new ProfilerFunction[PROFILE_SIZE];
	length = 0;

	stack = new uint32_t[PROFILE_SIZE];
	stackCurrent = 0;
}

ProfilerFunction* Profiler::findFunction(const char* name) {
	DART_NOT_NULL_RETURN(name, DART_ERROR_UTIL, NULL);
	if (functions == NULL)
		init();
	DART_NOT_NULL_RETURN(functions, DART_ERROR_INTERNAL, NULL);

	uint32_t index = UINT32_MAX;

	// Find entry
	for (uint32_t i = 0; i < length; i++) {
		if (strcmp(functions[i].name, name) == 0) {
			index = i;
			break;
		}
	}

	// No entry found
	if (index == UINT32_MAX) {
		if (length >= PROFILE_SIZE) {
			Log::error("Profiler", "Profiler buffer full");
			return NULL;
		}

		index = length;

		// Create new one
		ProfilerFunction* function = &functions[index];
		DART_NOT_NULL_RETURN(function, DART_ERROR_INTERNAL, NULL);

		memset(function, 0, sizeof(ProfilerFunction));
		function->index = index;
		function->name = (char*)name;
		length++;
		return function;
	}
	return &functions[index];
}

uint32_t Profiler::start(const char* name) {
	return start(name, true);
}

uint32_t Profiler::start(const char* name, bool reset) {
	if (!enabled)
		return UINT32_MAX;

	ProfilerFunction* function = findFunction(name);
	if (function == NULL)
		return UINT32_MAX;

	if (function->shouldReset && reset)
		function->time = 0;  // Reset time

	function->currentTime = highres_time_get();
	return function->index;
}

uint64_t Profiler::stop(ProfilerFunction* function, bool add) {
	if (!enabled)
		return 0;

	DART_NOT_NULL_RETURN(function, DART_ERROR_UTIL, 0);
	if (function->currentTime != 0) {
		uint64_t time = highres_time_get();
		if (time > function->currentTime) {
			uint64_t diff = time - function->currentTime;
			if (add)
				function->time += diff;
			else
				function->time = diff;
		}
	}
	return function->time;
}

uint64_t Profiler::stop(const char* name) {
	ProfilerFunction* function = findFunction(name);
	if (function == NULL)
		return 0;

	return stop(function, false);
}


void Profiler::begin(const char* name) {
	if (!enabled)
		return;

	uint32_t func = start(name);
	if (func != UINT32_MAX) {
		if (stackCurrent >= PROFILE_SIZE) {
			Log::error("Profiler", "Stack overflow");
			return;
		}
		stack[stackCurrent++] = func;
	}
}

uint64_t Profiler::end() {
	if (!enabled)
		return 0;

	if (functions == NULL) {
		Log::error("Profiler", "init() not called");
		return 0;
	}
	if (stackCurrent == 0) {
		Log::error("Profiler", "Invalid end(), no parts left");
		return 0;
	}

	uint32_t index = stack[--stackCurrent];
	return stop(&functions[index], true);
}

bool Profiler::end(bool returnValue) {
	Profiler::end();
	return returnValue;
}

void Profiler::restart(const char* name) {
	stop(name);
	start(name, false);
}

void Profiler::finishFrame() {
	if (!enabled)
		return;

	if (functions == NULL)
		init();

	DART_NOT_NULL(functions, DART_ERROR_INTERNAL);

	if (stackCurrent > 0) {
		Log::error("Profiler", "Frame not correctly ended");
		while (stackCurrent > 0)
			end();
	}

	bool resetMax = millis() - lastMaxReset > 8000;
	if (resetMax)
		lastMaxReset = millis();

	for (uint32_t i = 0; i < length; i++) {
		ProfilerFunction* function = &functions[i];
		DART_NOT_NULL(function, DART_ERROR_INTERNAL);

		function->lastTime = function->time;
		function->shouldReset = true;

		if (resetMax || function->time > function->maxTime)
			function->maxTime = function->time;
	}
}

void Profiler::setTime(const char* name, uint32_t value) {
	ProfilerFunction* function = findFunction(name);
	if (function == NULL)
		return;

	function->time = value;
}

void Profiler::setTimeMillis(const char* name, uint32_t value) {
	setTime(name, value * 1000);
}

#ifdef PACKETBUFFER_ENABLED
void Profiler::writeData(PacketBuffer* buffer) {
	DART_NOT_NULL(buffer, DART_ERROR_INVALID_VALUE);
	if (functions == NULL)
		init();

	DART_NOT_NULL(functions, DART_ERROR_INTERNAL);

	buffer->write(length);
	for (uint32_t i = 0; i < length; i++) {
		ProfilerFunction* function = &functions[i];
		DART_NOT_NULL(function, DART_ERROR_INTERNAL);

		buffer->writeString(function->name);
		buffer->write((uint32_t)function->lastTime);
		buffer->write((uint32_t)function->maxTime);
	}
}
#endif
