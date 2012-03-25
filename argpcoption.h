#ifndef __ARGPCOPTION__
#define __ARGPCOPTION__

#include <string>
#include <vector>

struct ArgpcGroup
{
    ArgpcGroup( std::string name );
    ArgpcGroup( std::string name, ArgpcGroup *child );
    ArgpcGroup( const ArgpcGroup &base );
    ~ArgpcGroup( );
    bool operator<(const ArgpcGroup &comp) const;
    std::string fullName( void ) const;
    

    std::string name;
    ArgpcGroup *child;
};

class ArgpcOption
{
public:
    friend class Argpc;
    ArgpcOption( std::string name, std::string doc, void (*callback)( void ) );
    ArgpcOption( std::string name, char id, std::string doc, void (*callback)( void ) );
    ArgpcOption( std::string name, char id, std::string value, std::string doc, void (*callback)( std::string ) );
    ArgpcOption( const ArgpcOption &base );
    const ArgpcOption &operator=(const ArgpcOption &base);
    bool operator<(const ArgpcOption &comp) const;
    ~ArgpcOption( void );

    void addValue( std::string name );
    void write( void );

private:
    /**
     * Outputs a string to stdout and returns the number of characters written
     * 
     * @param s The string to write
     * @return The number of characters written
     */
    int output( std::string s );

    std::string name;
    char id;
    std::string valueName;
    std::string doc;
    bool hasValue;
    std::vector< std::string > values;
    ArgpcGroup *group;
    union
    {
        void (*noarg)( void );
        void (*arg)( std::string );
    } callback;
};

#endif /* __ARGPCOPTION__ */
