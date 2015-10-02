#ifndef _ECHO_H_
#define _ECHO_H_

#include "Command.h"

class Echo : public Command
{  
public:
    virtual void initialize()
    {
        Command::initialize();
    }
    
    virtual void destroy()
    {
        Command::destroy();
    }
    
    virtual int parse()
    {
        return  0;
    }
    
    virtual int handle()
    {
        webpage page;
	page.load("/usr/local/grove/fast-cgi/html/query_user.html");
	page.output();
        return 0;
    }
};

#endif

