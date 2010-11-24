#include <db.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <list>
#include "exception.h"

using namespace std;

void clear_dependencies(unsigned char hash[32])
{
	DB *dbp;
	DBT key;
	u_int32_t flags;
	int ret;

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

	ret = dbp->open(dbp, NULL, "makefile.dep", NULL, DB_BTREE, flags, 0);
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

void add_dependencies(unsigned char hash[32], string dep, bool success)
{
	DBT key, data;
	DB *dbp;
	int ret;
	u_int32_t flags;

	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	key.data = hash;
	key.size = 32;

	data.data = (void *)dep.c_str();
	data.size = dep.length();

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

	ret = dbp->open(dbp, NULL, "makefile.dep", NULL, DB_BTREE, flags, 0);
	if( ret != 0 ) {
		dbp->close(dbp, 0);
		throw runtime_wexception("Failed to open database for write");
	}

	ret = dbp->put(dbp, NULL, &key, &data, 0);
	if( ret != 0 ) {
		throw runtime_wexception("Could not insert record");
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

	ret = dbp->open(dbp, NULL, "makefile.dep", NULL, DB_BTREE, flags, 0);
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
		deps->push_back(pair<string,bool>(string((const char *)data.data, (string::size_type)data.size), false));
		ret = cursor->get(cursor, &key, &data, DB_NEXT_DUP);
	}
	if( data.data != NULL ) {
		free( data.data );
	}

	dbp->close(dbp, 0);

	return deps;
}
