#pragma once

#include <ctime>
#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/standard_include.h"

class NMEAStyleParser {
private:
	char buffer[300];

	bool ignoreMultipleDollarSigns;
	bool ignoreChecksum;

public:
	explicit NMEAStyleParser();

	void setIgnoreMultipleDollarSigns(bool ignore);
	void setIgnoreChecksum(bool ignore);

	char* getBuffer();
	size_t getBufferSize();

	uint8_t oneDigit(char a);
	uint8_t twoDigitDec(char a, char b);
	uint8_t twoDigitHex(char a, char b);

	bool fillBuffer(const char* sentence);

	char* firstValue();
	char* nextValue();

	bool skipValue();

	bool parseInt(int* value);
	bool parseFloat(float* value);
	bool parseChar(char* value);

	bool parseTimeStamp(tm* value);
	bool parseDateStamp(tm* value);
	bool parseCoordinate(float* value);
};
