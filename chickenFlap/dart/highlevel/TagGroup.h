#pragma once

#include "../../../chickenFlap/dart/standard_include.h"
#include "../../../chickenFlap/dart/util/timer.h"

/**
 * Structure used by the data stream to save data about tags used by the logger.
 */
struct TagGroup {
	util_timer_t timer;
	uint16_t tagStart; // Index of the first tag saved in the tags list
};
