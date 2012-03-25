#ifndef __SUBPROCESS_H__
#define __SUBPROCESS_H__

#include <string>

class Subprocess
{
public:
    virtual ~Subprocess( );
    
    /* Execute a command, while calling callbacks on entry and exit to each
     * kernel filesystem access
     */
    void trace(std::string command);

    /* Callback when entering a filesystem access */
    virtual void callback_entry(std::string filename) = 0;
    
    /* Callback when leaving a filesystem access */
    virtual void callback_exit(std::string filename, bool success) = 0;
};

#endif /* __SUBPROCESS_H__ */
