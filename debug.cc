#include "debug.h"
#include "exception.h"
#include "argpc.h"

static bool debug_settings[DEBUG_LEVEL_MAX] = { 0 };

struct {
	const char *name;
	DebugLevel val;
} static debugLabels[] = {
	{ "dependencies", DEBUG_DEPENDENCIES },
	{ "subprocess", DEBUG_SUBPROCESS },
	{ "reason", DEBUG_REASON },
	{ "1", DEBUG_LEVEL_1 },
	{ "2", DEBUG_LEVEL_2 },
};

static void debug_mode( std::string option )
{
	unsigned int i;

	for( i = 0; i< sizeof(debugLabels)/sizeof(debugLabels[0]); i ++ ) {
		if( option == debugLabels[ i ].name ) {
			enable_debug( debugLabels[ i ].val );
			return;
		}
	}
}

void debug_init()
{
	unsigned int i;

	ArgpcOption debugOption( "debug", 'd', "debuglevel", "Ouput debugging information", debug_mode );
	for( i = 0; i< sizeof(debugLabels)/sizeof(debugLabels[0]); i ++ ) {
		debugOption.addValue( debugLabels[ i ].name );
	}

	Argpc::getInstance()->addOption( debugOption );
}

static void debug_set_to_level(DebugLevel level)
{
	int i;

	for( i = 0; i < level; i ++ ) {
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
