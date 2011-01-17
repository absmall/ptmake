#include <db.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <set>
#include "debug.h"
#include "exception.h"
#include <iostream>
#include <sstream>
#include "utilities.h"

using namespace std;

string depfile = "makefile.dep";

void clear_dependencies(unsigned char hash[32])
{
	DB *dbp;
	DBT key;
	u_int32_t flags;
	int ret;

	if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
		cout << "Clearing dependencies for " << printhash(hash) << endl;
	}

	ret = db_create( &dbp, NULL, 0);
	if( ret != 0 ) {
		throw runtime_wexception("Failed to create database");
	}

	dbp->set_flags(dbp, DB_DUPSORT);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to make database sorted");
	}

	flags = DB_CREATE;

	ret = dbp->open(dbp, NULL, depfile.c_str(), NULL, DB_BTREE, flags, 0);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to open database");
	}

	memset( &key, 0, sizeof(DBT) );

	key.data = hash;
	key.size = 32;
	key.flags = 0;

	ret = dbp->del(dbp, NULL, &key, 0);
	if( ret != 0 && ret != DB_NOTFOUND ) {
		dbp->close(dbp, 0);
		printf("ret=%s\n", db_strerror(ret));
		throw runtime_wexception("Failed to delete key");
	}

	dbp->close(dbp, 0);
}

void add_dependencies(unsigned char hash[32], const std::set<std::pair<std::string, bool> > &deps)
{
	DBT key, data;
	DB *dbp;
	int ret;
	u_int32_t flags;
	unsigned char *tempBuf;

	if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
		cout << "Adding list of dependencies for " << printhash(hash) << endl;
	}

	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	key.data = hash;
	key.size = 32;

	ret = db_create( &dbp, NULL, 0);
	if( ret != 0 ) {
		throw runtime_wexception("Failed to create database for write");
	}

	ret = dbp->set_flags(dbp, DB_DUPSORT);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to make database sorted for write");
	}

	flags = DB_CREATE;

	ret = dbp->open(dbp, NULL, depfile.c_str(), NULL, DB_BTREE, flags, 0);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to open database for write");
	}

	for( std::set<std::pair<std::string, bool> >::const_iterator i = deps.begin(); i != deps.end(); i ++ ) {
		tempBuf = (unsigned char *)malloc( i->first.length() + 2 );
		memcpy( tempBuf, i->first.c_str(), i->first.length() + 1 );
		tempBuf[ i->first.length() + 1 ] = i->second;
		data.data = (void *)tempBuf;
		data.size = i->first.length()+2;

		ret = dbp->put(dbp, NULL, &key, &data, 0);
		if( ret != 0 ) {
			throw runtime_wexception("Could not insert record");
		}

		free( tempBuf );
	}

	dbp->close(dbp, 0);
}

list<pair<string, bool> > *retrieve_dependencies(unsigned char hash[32])
{
	DB *dbp;
	DBC *cursor;
	DBT key, data;
	u_int32_t flags;
	int ret;
	list<pair<string, bool> > *deps = NULL;

	if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
		cout << "Retrieving dependencies for " << printhash(hash) << endl;
	}

	ret = db_create( &dbp, NULL, 0);
	if( ret != 0 ) {
		throw runtime_wexception("Failed to create database");
	}

	dbp->set_flags(dbp, DB_DUPSORT);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to make database sorted");
	}

	flags = DB_CREATE;

	ret = dbp->open(dbp, NULL, depfile.c_str(), NULL, DB_BTREE, flags, 0);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to open database");
	}

	dbp->cursor(dbp, NULL, &cursor, 0 );

	memset( &key, 0, sizeof(DBT) );
	memset( &data, 0, sizeof(DBT) );

	key.data = hash;
	key.size = 32;

	data.flags = DB_DBT_REALLOC;

	ret = cursor->get(cursor, &key, &data, DB_SET);
	while( ret != DB_NOTFOUND ) {
		if( deps == NULL ) {
			deps = new list<pair<string, bool> >;
		}
		deps->push_back(pair<string,bool>(string((const char *)data.data, (string::size_type)data.size - 2), ((char *)data.data)[ data.size - 1 ]));
		ret = cursor->get(cursor, &key, &data, DB_NEXT_DUP);
	}
	if( data.data != NULL ) {
		free( data.data );
	}

	dbp->close(dbp, 0);

	return deps;
}
