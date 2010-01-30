#include <string>
#include <stdexcept>
#include <aprpp.h>
#include <config.h>
#include <apr_dso.h>
#include <apr_general.h>
#include <apr_file_info.h>
#include <iostream>

AprPP::AprResource AprPP::resources;

AprPP::AprPP( void )
{
	apr_pool_create( &mp, NULL );
}

AprPP::~AprPP( void )
{
	apr_pool_destroy( mp );
}

apr_pool_t *AprPP::getPool( void )
{
	return mp;
}

AprPP::AprResource::AprResource( void )
{
	apr_initialize( );
}

AprPP::AprResource::~AprResource( void )
{
	apr_terminate( );
}

AprFile::AprFile( std::string filename, int flags )
{
	char errormsg[ 256 ];

	apr_status_t status = apr_file_open( &file, filename.c_str( ), flags | APR_BUFFERED, APR_OS_DEFAULT, getPool( ) );
	if( status != APR_SUCCESS )
	{
		throw std::runtime_error( apr_strerror( status, errormsg, 256 ) );
	}
}

AprFile::~AprFile( void )
{
	apr_file_close( file );
}

std::string AprFile::readLine( void )
{
	int i;
	// Read in 100 bytes at a time. This isn't a line length limit, we concatenate
	// reads if the line is over 100 bytes.
	char buf[ 100 ];
	std::string s;
	apr_size_t bytes; 

	while( !apr_file_eof( file ) )
	{
		apr_file_gets( buf, 100, file );
		// Append the new bytes to what we have so far
		s += std::string( buf );

		// See if we got an end-of-line
		if( strchr( buf, '\n' ) ) {
			// We now have the full string. Start at the end-of-line and delete linefeed
			// characters. Since we do line-based input, they're not necessary
			std::string::size_type position = s.find_last_not_of( " \t\r\n" );
			if( position == std::string::npos ) {
				// Nothing other than whitespace
				return std::string( "" );
			} else {
				s.erase( position + 1 );
				
				// Check for trailing backslash. If there are an odd number of trailing
				// backslashes, one is for the newline
				std::string::size_type non_backslash = s.find_last_not_of( '\\' );
				if( non_backslash == std::string::npos ) {
					// Everything in the string is a backslash. If the length of the string is
					// odd, the newline is escaped
					if( !(position & 1) ) {
						// There is an escaped newline
						s.erase( position );
						continue;
					}
				} else {
					// There are non-backslash characters. See if what's at the end is an odd
					// number of them.
					if( (position - non_backslash) & 1 )
					{
						s.erase( position );
						continue;
					}
				}
				return s;
			}
		}
	}

	// Reached end-of-file without a newline. Return what we have, even if it's
	// nothing
	return s;
}

bool AprFile::eof( void )
{
	return apr_file_eof( file ) == APR_EOF;
}
