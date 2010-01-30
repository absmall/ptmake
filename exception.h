#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>

class wexception
{
public:
	virtual const char *what( void ) const throw( ) = 0;
};

class runtime_wexception : public wexception
{
public:
	runtime_wexception( const std::string &s );
	const char *what( void ) const throw( );
private:
	std::string s;
};

#endif /* __EXCEPTION_H__ */
