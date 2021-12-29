#pragma once

#include "../../../chickenFlap/dart/highlevel/NMEAStyleParser.h"

// Henrik Kunzelmann 2017

struct NMEAData {
	bool valid;
	tm time;
	float latitude;
	float longitude;
	float altitude;
	float speed;
};

// Implements an NMEA protocol parser for reading GPS data
class NMEA : public NMEAStyleParser {
private:
	NMEAData lastData;

	bool parseBuffer();


public:
	explicit NMEA();

	bool parse(const char* sentence);

	bool isValid();
	time_t getTime();
	float getLatitude();
	float getLongitude();
	float getAltitude();
	float getSpeed();
};
