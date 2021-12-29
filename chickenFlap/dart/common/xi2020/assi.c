#include "../../../../chickenFlap/dart/common/xi2020/assi.h"

const char* assi_state_string(const ASSI_state_t state) {
	switch(state) {
		case ASSI_OFF:
			return "ASSI_OFF";
		case ASSI_YELLOW:
			return "ASSI_YELLOW";
		case ASSI_YELLOW_BLINK:
			return "ASSI_YELLOW_BLINK";
		case ASSI_BLUE:
			return "ASSI_BLUE";
		case ASSI_BLUE_BLINK:
			return "ASSI_BLUE_BLINK";
		case ASSI_BOTH:
			return "ASSI_BOTH";
		case ASSI_BOTH_BLINK:
			return "ASSI_BOTH_BLINK";
		case ASSI_BOTH_ALTERNATE:
			return "ASSI_BOTH_ALTERNATE";
		default:
			return "ASSI_UNKNOWN";
	}
}
