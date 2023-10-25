#ifndef DART_UTIL_GEO_DISTANCE_H_
#define DART_UTIL_GEO_DISTANCE_H_

#include "../../../chickenFlap/dart/util/math.h"

#ifdef __cplusplus
extern "C" {
#endif

bool geo_valid(float latitude, float longitude);
float geo_distance(float lat1, float long1, float lat2, float long2);

#ifdef __cplusplus
}
#endif

#endif
