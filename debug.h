#ifndef __DEBUG_H__
#define __DEBUG_H__

enum DebugLevel {
	DEBUG_NONE,

	DEBUG_DEPENDENCIES,
	DEBUG_LEVEL_1,

	DEBUG_SUBPROCESS,
	DEBUG_PARSE,
	DEBUG_LEVEL_2,

	DEBUG_LEVEL_MAX
};

bool enable_debug(DebugLevel level);

bool get_debug_level(DebugLevel level);

#endif
