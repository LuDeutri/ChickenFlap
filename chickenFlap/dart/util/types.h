/**
 * Defines common helper defines and types.
 */
#ifndef SRC_UTIL_TYPES_H_
#define SRC_UTIL_TYPES_H_

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __GNUC__
	#ifndef likely
	#define likely(x)    __builtin_expect(!!(x), 1)
	#endif

	#ifndef unlikely
	#define unlikely(x)  __builtin_expect(!!(x), 0)
	#endif
#else
	#ifndef likely
	#define likely(x)    x
	#endif

	#ifndef unlikely
	#define unlikely(x)    x
	#endif
#endif

/**
 * NULL is the null pointer.
 */
#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 * Define size_t.
 */
#ifndef _SIZE_T
#ifndef _SIZE_T_DECLARED
#define _SIZE_T
#define _SIZE_T_DECLARED
typedef __SIZE_T_TYPE__ size_t;
#endif
#endif

/**
 * Define offsetof
 */
#ifndef offsetof
#define offsetof(st, m) \
    ((size_t)((char *)&((st *)0)->m - (char *)0))
#endif

#endif
