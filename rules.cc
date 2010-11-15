#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include <gcrypt.h>
#include "file.h"
#include "rules.h"
#include "build.h"
#include "exception.h"
#include "subprocess.h"

using namespace std;

extern bool debug;
std::list<Rule *> Rule::rules;
std::set<std::string> Rule::buildCache;

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

void Rule::callback(std::string filename)
{
	if( debug ) {
		::print(filename);
	}
	try {
		try_to_build(filename);
	} catch( wexception &e ) {
		// Do nothing
	}
}

void Rule::execute()
{
	if( targets == NULL || commands == NULL ) return;
	try {
		targetTime = fileTimeEarliest( *targets );
	} catch( ... ) {
		// No target time, so definitely rebuild
	}
	for(list<string>::iterator i = commands->begin(); i != commands->end(); i ++ ) {
		trace(*i);
	}
}

Rule::Rule( )
{
	rules.push_back(this);
	targets = NULL;
	commands = NULL;
}

Rule *Rule::find(const string &target)
{
	Rule *r;
	bool foundARule = false;
	for(list<Rule *>::iterator i = rules.begin(); i != rules.end(); i ++ )
	{
		std::list<std::string>::iterator b, e;
		if( (*i)->targets == NULL ) continue;
		b = (*i)->targets->begin();
		e = (*i)->targets->end();
		if( e != std::find(b, e, target) ) {
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

void Rule::try_to_build(const string &target)
{
	// See if we have already built this
	if( buildCache.find(target) != buildCache.end() ) {
		// Already built
		return;
	}

	buildCache.insert(target);
	Rule *r = Rule::find(target);
	r->execute();
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

	hash = (string)(char *)gcry_md_read( hd, 0 );

	gcry_md_close( hd );
}
