#include "../../../chickenFlap/dart/highlevel/NMEAStyleParser.h"

NMEAStyleParser::NMEAStyleParser() {
	memset(buffer, 0, sizeof(buffer));
	ignoreMultipleDollarSigns = false;
	ignoreChecksum = false;
}

void NMEAStyleParser::setIgnoreMultipleDollarSigns(bool ignore) {
	ignoreMultipleDollarSigns = ignore;
}

void NMEAStyleParser::setIgnoreChecksum(bool ignore) {
	ignoreChecksum = ignore;
}

char* NMEAStyleParser::getBuffer() {
	return buffer;
}

size_t NMEAStyleParser::getBufferSize() {
	return sizeof(buffer);
}

uint8_t NMEAStyleParser::oneDigit(char a)
{
	if (a >= '0' && a <= '9')
		return (a - '0');
	if (a >= 'a' && a <= 'f')
		return (a - 'a') + 10;
	if (a >= 'A' && a <= 'F')
		return (a - 'A') + 10;
	return 0;
}

uint8_t NMEAStyleParser::twoDigitDec(char a, char b)
{
	return oneDigit(a) * 10 + oneDigit(b);
}

uint8_t NMEAStyleParser::twoDigitHex(char a, char b)
{
	return oneDigit(a) * 16 + oneDigit(b);
}

bool NMEAStyleParser::fillBuffer(const char* sentence)
{
	if (sentence == NULL)
		return false;
	if (strlen(sentence) >= sizeof(buffer))
		return false;

	int length = strlen(sentence);
	int start = -1;
	int end = -1;

	for (int i = 0; i < length; i++) {
		if (sentence[i] == '$') {
			if (start != -1 && !ignoreMultipleDollarSigns) // parsed already a $ char
				return false;
			start = i + 1;
		}
		else if (sentence[i] == '*') {
			if (end != -1) // parsed already a * char
				return false;
			end = i;
		}
	}

	// $ is always before *, otherwise indicate error
	if (start >= end) {
		if (ignoreChecksum && end == -1)
			end = length;
		else
			return false;
	}

	// Read checksum after message
	int checksumLength = (length - end - 1);

	if (checksumLength > 0 && !ignoreChecksum) {
		// Checksum should be two digits
		if (checksumLength != 2)
			return false;

		if (end + 2 >= length)
			return false;

		// Last two digits are the checksum in hex
		uint8_t messageChecksum = twoDigitHex(sentence[end + 1], sentence[end + 2]);
		uint8_t calculatedChecksum = 0;

		// Calculate the checksum of the message to compare it
		for (int i = start; i < end; i++)
			calculatedChecksum ^= sentence[i];

		// Checksum mismatch
		if (messageChecksum != calculatedChecksum)
			return false;
	}

	int msgLength = end - start;

	// Copy to internal buffer
	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, sentence + start, msgLength);
	return true;
}

char* NMEAStyleParser::firstValue()
{
	return strtok(buffer, ",");
}

char* NMEAStyleParser::nextValue()
{
	return strtok(NULL, ",");
}

bool NMEAStyleParser::skipValue()
{
	return nextValue() != NULL;
}

bool NMEAStyleParser::parseInt(int* value)
{
	char* str = nextValue();
	if (str == NULL)
		return false;

	if (sscanf(str, "%d", value) == 0)
		return false;
	return true;
}

bool NMEAStyleParser::parseFloat(float* value)
{
	char* str = nextValue();
	if (str == NULL)
		return false;

	if (sscanf(str, "%f", value) == 0)
		return false;
	return true;
}

bool NMEAStyleParser::parseChar(char* value)
{
	char* str = nextValue();
	if (str == NULL)
		return false;

	if (strlen(str) != 1)
		return false;
	*value = str[0];
	return true;
}

bool NMEAStyleParser::parseTimeStamp(tm* tm)
{
	char* str = nextValue();
	if (str == NULL)
		return false;

	if (strlen(str) < 6)
		return false;

	tm->tm_hour = twoDigitDec(str[0], str[1]);
	tm->tm_min = twoDigitDec(str[2], str[3]);
	tm->tm_sec = twoDigitDec(str[4], str[5]);
	return true;
}

bool NMEAStyleParser::parseDateStamp(tm* tm)
{
	char* str = nextValue();
	if (str == NULL)
		return false;

	if (strlen(str) < 6)
		return false;

	tm->tm_mday = twoDigitDec(str[0], str[1]);
	tm->tm_mon = twoDigitDec(str[2], str[3]) - 1;

	// Calculate years since 1900
	int year = twoDigitDec(str[4], str[5]);
	if (year < 60)
		year = 100 + year;

	tm->tm_year = year;
	tm->tm_isdst = -1;
	return true;
}

bool NMEAStyleParser::parseCoordinate(float* value)
{
	float v;
	if (!parseFloat(&v))
		return false;

	char dir;
	if (!parseChar(&dir))
		return false;

	int sign = 0;

	if (dir == 'n' || dir == 'e' || dir == 'N' || dir == 'E')
		sign = 1;
	else if (dir == 's' || dir == 'w' || dir == 'S' || dir == 'W')
		sign = -1;
	else
		return false;

	// Convert to decimal format
	v = fabs(v);
	float degress = ((int)v) / 100;
	float minutes = fmod(v, 100);

	*value = (degress + minutes / 60) * sign;
	return true;
}
