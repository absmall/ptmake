#ifndef __APRPP_H__
#define __APRPP_H__

#include <string>
#include <apr_general.h>
#include <apr_file_io.h>

class AprPP
{
public:
	AprPP( void );
	~AprPP( void );

	/**
	 *
	 */
	

	apr_pool_t *getPool( void );

private:
	class AprResource
	{
	public:
		AprResource( void );
		~AprResource( void );
	};
	static AprResource resources;

	apr_pool_t *mp;
};

class AprFile : public AprPP
{
public:
	AprFile( std::string, int flags );
	~AprFile( );
	std::string readLine( void );
	bool eof( void );
private:
	apr_file_t *file;
};


#endif
