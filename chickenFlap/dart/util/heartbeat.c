#include "../../../chickenFlap/dart/util/heartbeat.h"

heartbeat_t createHeartbeat(util_time_t timeout) {
	heartbeat_t heartbeat;
	heartbeat.lastTrigger = millis();
	heartbeat.timeout = timeout;
	return heartbeat;
}

void resetHeartbeat(heartbeat_t* heartbeat) {
	DART_NOT_NULL(heartbeat, DART_ERROR_UTIL);
	heartbeat->lastTrigger = millis();
}

bool isHeartbeatLost(heartbeat_t heartbeat) {
	return hasTimeElapsed(heartbeat.lastTrigger, heartbeat.timeout);
}
