#include "../../../chickenFlap/dart/util/error.h"

const char* DART_ERROR_STR(int errorCode) {
	switch(errorCode) {
	case DART_ERROR_UNKNOWN:
	    return "DART_ERROR_UNKNOWN";
	case DART_ERROR_INTERNAL:
	    return "DART_ERROR_INTERNAL";
	case DART_ERROR_INVALID_VALUE:
	    return "DART_ERROR_INVALID_VALUE";
	case DART_ERROR_HAL:
	    return "DART_ERROR_HAL";
	case DART_ERROR_UTIL:
	    return "DART_ERROR_UTIL";
	case DART_ERROR_BUFFER_OVERFLOW:
	    return "DART_ERROR_BUFFER_OVERFLOW";
	case DART_ERROR_ID_ALREADY_IN_USE:
	    return "DART_ERROR_ID_ALREADY_IN_USE";
	case DART_ERROR_NOT_IMPLEMENTED:
	    return "DART_ERROR_NOT_IMPLEMENTED";
	case DART_ERROR_TIMEOUT:
	    return "DART_ERROR_TIMEOUT";
	case DART_ERROR_VALUE_IS_NULL:
	    return "DART_ERROR_VALUE_IS_NULL";
	}
	return NULL;
}

void dart_error_wrapper(int errorCode, const char* file, int line) {
    #ifdef USE_NEW_DART_ERROR
        dart_error(errorCode, file, line);
    #else
        DART_NOT_USED(file);
        DART_NOT_USED(line);
        //dart_error(errorCode);
    #endif
}
