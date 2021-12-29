#include "../../../chickenFlap/dart/highlevel/NMEA.h"

NMEA::NMEA() : NMEAStyleParser()
{
	memset(&lastData, 0, sizeof(lastData));
}

bool NMEA::parse(const char* sentence)
{
	if (!fillBuffer(sentence))
		return false;
	return parseBuffer();
}

bool NMEA::isValid()
{
	return lastData.valid;
}

time_t NMEA::getTime()
{
	tm time = lastData.time;
	return mktime(&time);
}

float NMEA::getLatitude()
{
	return lastData.latitude;
}

float NMEA::getLongitude()
{
	return lastData.longitude;
}

float NMEA::getAltitude()
{
	return lastData.altitude;
}

float NMEA::getSpeed()
{
	// Speed is in knots... convert to km/h
	return lastData.speed * 1.852;
}



bool NMEA::parseBuffer()
{
	char* type = firstValue();
	if (type == NULL)
		return false;

	NMEAData data = lastData;
	if (strcmp(type, "GPRMC") == 0) {
		char validStatus;
		if (!parseTimeStamp(&data.time))
			return false;
		if (!parseChar(&validStatus))
			return false;
		if (!parseCoordinate(&data.latitude))
			return false;
		if (!parseCoordinate(&data.longitude))
			return false;
		if (!parseFloat(&data.speed))
			return false;
		if (!skipValue()) // direction of movement
			return false;
		if (!parseDateStamp(&data.time))
			return false;
		// Other values ignored

		data.valid = validStatus == 'A';
	}
	else if (strcmp(type, "GPGGA") == 0) {
		int quality;
		if (!parseTimeStamp(&data.time))
			return false;
		if (!parseCoordinate(&data.latitude))
			return false;
		if (!parseCoordinate(&data.longitude))
			return false;
		if (!parseInt(&quality))
			return false;
		if (!skipValue()) // numbers of satellites
			return false;
		if (!skipValue()) // HDOP
			return false;
		if (!parseFloat(&data.altitude))
			return false;
		// Other values ignored

		data.valid = quality > 0;
	}
	else {
		// Unknown message type
		// Return true, because the message is probably valid, but it is not supported
		return true;
	}

	// Data correctly parsed
	lastData = data;
	return true;
}

