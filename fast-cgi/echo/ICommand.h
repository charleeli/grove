#ifndef _ICOMMAND_H_
#define _ICOMMAND_H_

using google::protobuf::Message;
using namespace std;

class ICommand
{
    protected:
        int checkToken(const ReqHead& head);
	
    public:
        inline ICommand() {};
        virtual ~ICommand() {};

        virtual void initialize() = 0;
        virtual void destroy() = 0; 
        virtual int parse(const ReqHead& head) = 0;
        virtual int handle(const string& in, string& out) = 0;
};

int ICommand::checkToken(const ReqHead& head)
{
    return 0;
}

#endif

