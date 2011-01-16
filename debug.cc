#include "debug.h"
#include "exception.h"

static bool debug_settings[DEBUG_LEVEL_MAX] = { 0 };

static void debug_set_to_level(DebugLevel level)
{
	int i;

	for( i = 0; i < DEBUG_LEVEL_2; i ++ ) {
		debug_settings[ i ] = true;
	}
	for( ; i < DEBUG_LEVEL_MAX; i ++ ) {
		debug_settings[ i ] = false;
	}
}

bool enable_debug(DebugLevel level)
{
	if( level >= DEBUG_LEVEL_MAX ) throw runtime_wexception( "Invalid debug level" );
	switch(level) {
		case DEBUG_LEVEL_1:
		case DEBUG_LEVEL_2:
			debug_set_to_level( level );
			break;
		default:
			debug_settings[ level ] = true;
			break;
	}

	return true;
}

bool get_debug_level(DebugLevel level)
{
	if( level >= DEBUG_LEVEL_MAX ) throw runtime_wexception( "Invalid debug level" );
	return debug_settings[ level ];
}
