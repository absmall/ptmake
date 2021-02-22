#include "argpcoption.h"

#include <stddef.h>
#include <iostream>

using namespace std;

ArgpcOption::ArgpcOption( std::basic_string<char> name, std::basic_string<char> doc, void (*callback)( void ) ) : name( name ), id( 0 ), doc( doc ), hasValue( false ), group( NULL )

{
    this->callback.noarg = callback;
}

ArgpcOption::ArgpcOption( basic_string<char> name, char id, basic_string<char> doc, void (*callback)( void ) ) : name( name ), id( id ), doc( doc ), hasValue( false ), group( NULL )
{
    this->callback.noarg = callback;
}

ArgpcOption::ArgpcOption( basic_string<char> name, char id, basic_string<char> value, basic_string<char> doc, void (*callback)( std::basic_string<char> ) ) : name( name ), id( id ), valueName( value ), doc( doc ), hasValue( true ), group( NULL )
{
    this->callback.arg = callback;
}

ArgpcOption::ArgpcOption( const ArgpcOption &base )
{
    group = NULL;
    *this = base;
}

const ArgpcOption &ArgpcOption::operator=(const ArgpcOption &base)
{
    name = base.name;
    id = base.id;
    valueName = base.valueName;
    doc = base.doc;
    hasValue = base.hasValue;
    values = base.values;
    if( group )
    {
        delete group;
    }

    if( base.group )
    {
        group = new ArgpcGroup( *base.group );
    } else {
        group = NULL;
    }
    callback = base.callback;

    return *this;
}

bool ArgpcOption::operator<(const ArgpcOption &comp) const
{
    // First show non-grouped items
    if( group == NULL && comp.group != NULL ) return true;
    if( group != NULL && comp.group == NULL ) return false;

    // Then sort by group
    if( group != NULL && comp.group != NULL)
    {
        if( group < comp.group ) return true;
        if( comp.group < group ) return false;
    }

    // and finally sort on name
    return name < comp.name;
}

ArgpcOption::~ArgpcOption( void )
{
    if( group )
    {
        delete group;
    }
}

void ArgpcOption::addValue( basic_string<char> name )
{
    values.push_back( name );
}

void ArgpcOption::write( void )
{
    int length = 0;

    length += output( "  " );

    // Print the ID if there is one
    if( id )
    {
        length += output( "-" );
        length += output( basic_string<char>( &id, 1 ) );
        length += output( ", " );
    } else {
        cout << "    ";
    }

    // Print the long option 
    if( name  != basic_string<char>( "" ) ) {
        cout << "--" << name;
    } else {
        cout << " ";
    }
}

int ArgpcOption::output( basic_string<char> s )
{
    cout << s;

    return s.size( );
}

ArgpcGroup::ArgpcGroup( basic_string<char> name ) : name( name ), child( NULL )
{
}

ArgpcGroup::ArgpcGroup( basic_string<char> name, ArgpcGroup *child ) : name( name ), child( child )
{
}

ArgpcGroup::ArgpcGroup( const ArgpcGroup &base )
{
    name = base.name;
    if( base.child )
    {
        child = new ArgpcGroup( *base.child );
    } else {
        child = NULL;
    }
}

ArgpcGroup::~ArgpcGroup( )
{
    if( child )
    {
        delete child;
    }
}

bool ArgpcGroup::operator<(const ArgpcGroup &comp) const
{
    // Sort on name
    if( name < comp.name ) return true;
    if( name > comp.name ) return false;

    // If both have children, subsort on children
    if( child != NULL && comp.child != NULL ) return *child < *comp.child;

    // If this has no child and comp does, that's the only way < can be true
    return child == NULL && comp.child != NULL;
}

std::basic_string<char> ArgpcGroup::fullName( void ) const
{
    if( child != NULL )
    {
        return child->fullName( ) + ":" + name;
    } else {
        return name;
    }
}
