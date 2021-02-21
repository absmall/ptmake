#include <sqlite3.h>
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
static DB *dbp;

void dependencies_init()
{
    int ret;
    u_int32_t flags;

#ifdef DARWIN
    dbp = dbopen( &dbp, NULL, 0);
    if( dbp != nullptr ) {
        throw runtime_wexception("Failed to create database");
    }
#else
    ret = db_create( &dbp, NULL, 0);
    if( ret != 0 ) {
        throw runtime_wexception("Failed to create database");
    }
#endif

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
}

void dependencies_deinit()
{
    dbp->close(dbp, 0);
}

void dependencies_reset()
{
    dependencies_deinit();
    unlink(depfile.c_str());
    dependencies_init();
}

void clear_dependencies(const unsigned char hash[32])
{
    DBT key;
    int ret;

    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        cout << "Clearing dependencies for " << printhash(hash) << endl;
    }

    memset( &key, 0, sizeof(DBT) );

    key.data = (unsigned char *)hash;
    key.size = 32;
    key.flags = 0;

    ret = dbp->del(dbp, NULL, &key, 0);

    // Database corrupt
    if( ret == DB_PAGE_NOTFOUND ) {
        dependencies_reset();
    } else if( ret != 0 && ret != DB_NOTFOUND ) {
        printf("ret=%s\n", db_strerror(ret));
        throw runtime_wexception("Failed to delete key");
    }
}

void add_dependencies(const unsigned char hash[32], const std::set<std::pair<std::string, bool> > &deps)
{
    DBT key, data;
    int ret;
    unsigned char *tempBuf;

    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        cout << "Adding list of dependencies for " << printhash(hash) << endl;
    }

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    key.data = (unsigned char *)hash;
    key.size = 32;

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
}

list<pair<string, bool> > *retrieve_dependencies(const unsigned char hash[32])
{
    DBC *cursor;
    DBT key, data;
    int ret;
    list<pair<string, bool> > *deps = NULL;

    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        cout << "Retrieving dependencies for " << printhash(hash) << endl;
    }

    dbp->cursor(dbp, NULL, &cursor, 0 );

    memset( &key, 0, sizeof(DBT) );
    memset( &data, 0, sizeof(DBT) );

    key.data = (unsigned char *)hash;
    key.size = 32;

    data.flags = DB_DBT_REALLOC;

    ret = cursor->get(cursor, &key, &data, DB_SET);
    while( ret == 0 ) {
        if( deps == NULL ) {
            deps = new list<pair<string, bool> >;
        }
        deps->push_back(pair<string,bool>(string((const char *)data.data, (string::size_type)data.size - 2), ((char *)data.data)[ data.size - 1 ]));
        ret = cursor->get(cursor, &key, &data, DB_NEXT_DUP);
    }
    if( data.data != NULL ) {
        free( data.data );
    }

    return deps;
}
