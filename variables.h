#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include <string>
#include <map>

class Variable
{
    public:
        virtual void Set( std::string ) = 0;

        virtual std::string Get( ) = 0;
};

class Scope
{
public:
    Scope();
    std::string lookup( std::string );
    ~Scope();
    
private:
    std::map< std::string, Variable * > variables;
    Scope *parent;
};

#endif /* __VARIABLES_H__ */
