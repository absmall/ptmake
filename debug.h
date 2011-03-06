#ifndef __DEBUG_H__
#define __DEBUG_H__

enum DebugLevel {
	DEBUG_NONE,

	DEBUG_REASON,
	DEBUG_LEVEL_1,

	DEBUG_DEPENDENCIES,
	DEBUG_SUBPROCESS,
	DEBUG_LEVEL_2,

	DEBUG_LEVEL_MAX
};

void debug_init( );

bool enable_debug(DebugLevel level);

bool get_debug_level(DebugLevel level);

#endif
