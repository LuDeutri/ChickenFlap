#include "../../../chickenFlap/dart/util/geo_distance.h"

bool geo_valid(float latitude, float longitude) {
	return geo_distance(latitude, longitude, 0, 0) > 10000;
}

float geo_distance(float lat1, float long1, float lat2, float long2) {
	const float R = 6371e3; // m
	float phi1 = deg_to_rad(lat1);
	float phi2 = deg_to_rad(lat2);
	float dPhi = deg_to_rad(lat2 - lat1);
	float dLambda = deg_to_rad(long2 - long1);

	float a = sinf(dPhi * 0.5f) * sinf(dPhi * 0.5f) +
			  cosf(phi1) * cosf(phi2) *
			  sinf(dLambda * 0.5f) * sinf(dLambda * 0.5f);
	return R * 2 * atan2f(sqrtf(a), sqrtf(1.0f - a));
}
