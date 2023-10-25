/**
 * Implements timers that work on the functions given in time.h (millis).
 * initTime (time.h) MUST be called before any other timer function can be used.
 */
#ifndef SRC_UTIL_TIMER_H_
#define SRC_UTIL_TIMER_H_

#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/time.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_OFFSET_RANDOM 0


/**
 * Structure for a timer. A timer is defined trough the period in milliseconds and the offset in milliseconds. The period defines the interval in which the
 * timer is elapsing. A period of 5, means the timer "ticks" every 5 millisecond. The offset in millisecond delays the timer at the start, so all timers
 * are running at different times and not all at once.
 */
typedef struct timer {
    /**
     * Internal value, saves the time when the timer has last ticked.
     * MUST be only changed by createTimer and hasTimerElapsed.
     */
	util_time_t _lastTime;
    /**
     * The period in milliseconds.
     */
	util_time_t period;
    /**
     * The offset in milliseconds.
     */
	util_time_t offset;

	/**
	 * Is this the first update of the timer?
	 */
	bool first;
} util_timer_t;

/**
 * Creates a new timer, defined by this period in milliseconds and offset in milliseconds.
 * The first tick of the timer is after (period + offset) milliseconds.
 * initTime (time.h) MUST be called before createTimer can be used.
 */
util_timer_t createTimer(util_time_t period, util_time_t offset);

/**
 * Creates a new timer, defined by this period in milliseconds.
 * The first tick of the timer is after period milliseconds.
 * initTime (time.h) MUST be called before createTimer can be used.
 */
util_timer_t createTimerNoOffset(util_time_t period);

/**
 * Resets the current time saved in the timer, i.e. the timer will elapse again in period milliseconds.
 */
void resetTimer(util_timer_t* timer);

/**
 * Checks whether the timer has elapsed ("ticked"), meaning one (or more) periods are over. If the
 * timer ticked, it is updated, so it will tick again in period milliseconds.
 * Returns true when the timer has elapsed, otherwise false is returned.
 * initTime (time.h) MUST be called before hasTimerElapsed can be used.
 */
bool hasTimerElapsed(util_timer_t* timer);

#ifdef __cplusplus
}
#endif

#endif
