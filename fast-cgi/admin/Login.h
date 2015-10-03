#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Command.h"

class Login : public Command
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

	if(m_param["username"]!="charleeli" || m_param["password"]!="717066513")
	{
		page.load("/usr/local/grove/fast-cgi/html/login.html");
		page.set("errmsg","username or password error!");
		page.output();
		return 0;
	}

	page.load("/usr/local/grove/fast-cgi/html/index.html");
	page.set("prompt","Welcome to index page!");
	page.set_bloc("INDEX");
	page.output();
        return 0;
    }
};

#endif

