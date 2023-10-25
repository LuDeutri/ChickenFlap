/**
 * Implements heartbeats that work on the functions given in time.h (millis).
 * initTime (time.h) MUST be called before any other timer function can be used.
 */
#ifndef DART_UTIL_HEARTBEAT_H_
#define DART_UTIL_HEARTBEAT_H_

#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/time.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	util_time_t lastTrigger;	// Last time the heartbeat was reset
	util_time_t timeout;		// The timeout in milliseconds
} heartbeat_t;

/**
 * Creates an heartbeat that elapses on the specified timeout in milliseconds.
 */
heartbeat_t createHeartbeat(util_time_t timeout);

/**
 * Resets the heartbeat, i.e. resets the timeout. See lastTrigger.
 */
void resetHeartbeat(heartbeat_t* heartbeat);

/**
 * Checks if the heartbeat elapsed.
 * Returns true when the heartbeat elapsed and false otherwise.
 */
bool isHeartbeatLost(heartbeat_t heartbeat);

#ifdef __cplusplus
}
#endif

#endif
