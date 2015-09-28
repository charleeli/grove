#ifndef _ECHO_H_
#define _ECHO_H_

#include "Command.h"
#include "Echo.pb.h"

using namespace pbjson;

class Echo : public Command
{
private:
    bool is_plaintext;//明文传输
    string uid;
    
public:
    virtual void initialize()
    {
        Command::initialize();
    }
    
    virtual void destroy()
    {
        Command::destroy();
    }
    
    virtual int parse(const ReqHead& head)
    {
        is_plaintext = head.plaintext().empty()?false:true;//是否明文
        uid = head.uid();
        return uid.empty() ? ERR_UID_UNLAW : 0;
    }
    
    virtual int handle(const string& in,string& out)
    {
        EchoReq reqBody;
        EchoRsp rspBody;
        
        if(is_plaintext)
		{
			string err;
            json2pb(in, &reqBody, err);
		}
		else
		{
			if (!reqBody.ParseFromString(in))
		    {
		        LOG(INFO)<<uid << " parse fail";
		        return ERR_PARSE_FAIL;
		    }
		}
        
        rspBody.set_foo(reqBody.foo());
    
        if(is_plaintext)
		{
        	pb2json(&rspBody, out);
		}
		else
		{
			out = rspBody.SerializeAsString();
		}
        
        //LOG(INFO)<<uid << "|" << reqBody.ShortDebugString() << "|" << rspBody.ShortDebugString();
        return 0;
    }
};

#endif

