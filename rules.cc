#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include <gcrypt.h>
#include "re.h"
#include "file.h"
#include "rules.h"
#include "build.h"
#include "debug.h"
#include "parse.h"
#include "exception.h"
#include "subprocess.h"
#include "dependencies.h"

using namespace std;

list<Rule *> Rule::rules;

void Rule::print()
{
	cout << "Rule:" << endl;

	if( targets != NULL ) {
		cout << "Targets:" << endl;
		for(list<string>::iterator i = targets->begin();
				i != targets->end();
				i ++)
		{
			cout << (*i);
		}
		cout << endl;
	}
	if( commands != NULL ) {
		cout << "Commands:" << endl;
		for(list<string>::iterator i = commands->begin();
				i != commands->end();
				i ++ )
		{
			cout << (*i) << endl;
		}
		cout << endl;
	}
}

void print(std::string filename)
{
	cout << "Depends on " << filename << endl;
}

void print(std::string filename, int status)
{
	cout << "Depends on " << filename << "(" << status << ")" << endl;
}

void Rule::callback_entry(std::string filename)
{
	if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
		::print(filename);
	}
	try {
		Rule *r = Rule::find(filename);
		r->execute();
		dependencies.insert( pair<string,bool>(filename, true) );
	} catch( wexception &e ) {
		// Do nothing
	}
}

void Rule::callback_exit(std::string filename, bool success)
{
	dependencies.insert( pair<string,bool>(filename, success) );

	if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
		::print(filename, success);
	}
}

bool Rule::execute()
{
	bool needsRebuild = false;
	list<pair<string, bool> > *deps;

	if( built ) return false;
	built = true;
	if( targets == NULL || commands == NULL ) return false;
	try {
		targetTime = fileTimeEarliest( *targets );
	} catch( ... ) {
		// No target time, so definitely rebuild
	}

	if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
		cout << "Try to build: " << *targets->begin() << "(" << targetTime << ")" << endl;
	}

	// See if we have dependencies in the database
	deps = retrieve_dependencies( hash );
	// If we know the dependencies, we may be able to avoid building. If we
	// don't know the dependencies, we definitely have to rebuild.
	if( deps != NULL ) {
		for( list<pair<string, bool> >::iterator i = deps->begin(); i != deps->end(); i ++ ) {
			// If the file has a rule, we need to try to rebuild it, and rebuild if that
			// succeeds.
			// If the file doesn't have a rule, then:
			// If the file didn't exist before, and it still doesn't, we don't need to
			// rebuild.
			// If the file didn't exist before, and it does now, we need to rebuild.
			// If the file existed before and doesn't exist now, we need to rebuild.
			// If the file existed before and still exists, we need to rebuild if the
			// file is newer
			if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
				cout << "Dependency " << i->first << "(" << i->second << ")" << endl;
			}
			try {
				// Use a rule to rebuild
				Rule *r = Rule::find(i->first);
				if( r->execute() ) {
					// It was rebuilt, so we need to rebuild the primary target
					needsRebuild = true;
				} else {
					bool status;
					time_t t;
					bool isDir;
					// If it wasn't rebuilt, but is already newer, we still
					// have to rebuild.
					status = fileTime(i->first, t, &isDir);
					if( !status || (t > targetTime && !isDir) ) {
						if( get_debug_level( DEBUG_REASON ) ) {
							cout << "Generated file out of date, need to rebuild because of " << i->first << ": (" << status << " ^ " << i->second << ") || " << ctime(&t) << " > " << ctime(&targetTime) << endl;
						}
						needsRebuild = true;
					}
				}
			} catch (...) {
				bool status;
				time_t t;
				bool isDir;

				status = fileTime(i->first, t, &isDir);
				if( (status ^ i->second) || (status && t > targetTime && !isDir) ) {
					if( get_debug_level( DEBUG_REASON ) ) {
						cout << "No rule to rebuild " << i->first << ": (" << status << " ^ " << i->second << ") || " << ctime(&t) << "(" << t << ")" << " > " << ctime(&targetTime) << "(" << targetTime << ")" << endl;
					}
					needsRebuild = true;
				}
			}
		}
		delete deps;

		if( !needsRebuild ) {
			return false;
		}
		if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
			cout << "Dependency updated, must build" << endl;
		}
	} else {
		// We don't know the dependencies, have to
		// build
		if( get_debug_level( DEBUG_REASON ) ) {
			cout << "Dependencies unknown, must build" << endl;
		}
	}
	
	clear_dependencies( hash );
	for(list<string >::iterator i = commands->begin(); i != commands->end(); i ++ ) {
		trace( expand_command( *i ) );

		// Touch the targets in case something else updated last in the build process 
	}
	add_dependencies( hash, dependencies );
	dependencies.clear();

	return true;
}

Rule::Rule( )
{
	rules.push_back(this);
	targets = NULL;
	commands = NULL;
	built = false;
}

Rule *Rule::find(const string &target)
{
	Rule *r;
	bool foundARule = false;
	
	for(list<Rule *>::iterator i = rules.begin(); i != rules.end(); i ++ )
	{
		if( (*i)->match( target ) ) {
			if( foundARule ) {
				// We have multiple rules to build the target
				throw runtime_wexception( "Multiple rules" );
			}
			foundARule = true;
			r = *i;
		}
	}

	if( foundARule ) {
		return r;
	} else {
		throw runtime_wexception( "No rule" );
	}
}

bool Rule::match(const std::string &target)
{
	std::list<std::string>::iterator b, e, te;

	if( targets == NULL ) return false;

	b = targets->begin();
	e = targets->end();
	for( te = b; te != e; te ++ ) {
		if( ::match( *te, target ) ) {
			return true;
		}
	}
	return false;
}

void Rule::addTarget(const std::string &target)
{
	if( targets == NULL ) {
		targets = new list<string>;
	}
	targets->push_back(target);

	recalcHash();
}

void Rule::addTargetList(std::list<std::string> *targetList)
{
	if( targets != NULL ) {
		delete targets;
	}

	targets = targetList;

	recalcHash();
}

void Rule::addCommand(const std::string &command)
{
	if( commands == NULL ) {
		commands = new list<string>;
	}
	commands->push_back(command);

	recalcHash();
}

void Rule::addCommandList(std::list<std::string> *commandList)
{
	if( commands != NULL ) {
		delete commands;
	}

	commands = commandList;

	recalcHash();
}

void Rule::setDefaultTargets(void)
{
	if( !rules.empty() ) {
		Rule *r = *rules.begin();
		if( r->targets == NULL ) throw runtime_wexception("First rule has no targets");
		for(list<string>::iterator i = r->targets->begin(); i != r->targets->end(); i ++ ) {
			set_target(*i);
		}
	}
}

string Rule::expand_command( const string &command )
{
	return command;
}

void Rule::recalcHash(void)
{
	gcry_md_hd_t hd;

	gcry_md_open( &hd, GCRY_MD_SHA256, 0);
	if( targets != NULL ) {
		for(list<string>::iterator i = targets->begin(); i != targets->end(); i ++ ) {
			gcry_md_write( hd, i->c_str(), i->size() );
		}
	}

	if (commands != NULL ) {
		for(list<string>::iterator i = commands->begin(); i != commands->end(); i ++ ) {
			gcry_md_write( hd, i->c_str(), i->size() );
		}
	}
	gcry_md_final( hd );

	memcpy( hash, gcry_md_read(hd, 0), 32 );

	gcry_md_close( hd );
}
