#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "./sdk-cpp/Head.pb.h"

using google::protobuf::Message;
using namespace Grove;
using namespace std;

class Command
{
    protected:
        int checkToken(const ReqHead& head);
    
    public:
        inline Command() {};
        virtual ~Command() {};

        virtual void initialize();
        virtual void destroy(); 
        virtual int parse(const ReqHead& head) = 0;
        virtual int handle(const string& in, string& out) = 0;
};

void Command::initialize()
{

}

void Command::destroy()
{

}

int Command::checkToken(const ReqHead& head)
{
    return 0;
}

#endif

