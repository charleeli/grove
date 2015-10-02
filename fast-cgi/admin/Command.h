#ifndef _COMMAND_H_
#define _COMMAND_H_

using namespace std;

class Command
{
    public:
        inline Command() {};
        virtual ~Command() {};

        virtual void initialize();
        virtual void destroy(); 
        virtual int parse() = 0;
        virtual int handle() = 0;
};

void Command::initialize()
{

}

void Command::destroy()
{

}

#endif

