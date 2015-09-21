#ifndef _ECHOREDIS_H_
#define _ECHOREDIS_H_

#include "Command.h"
#include "EchoWrapper.h"

using namespace pbjson;

class EchoRedis : public Command
{
private:
    bool is_plaintext;//明文传输
    string uid;
    
    EchoWrapper* pEcho;
    
public:
    virtual void initialize()
    {
        Command::initialize();
        pEcho = new EchoWrapper();
    }
    
    virtual void destroy()
    {
        Command::destroy();
        if(pEcho)
        {
            delete pEcho;
            pEcho = NULL;
        }
    }
    
    virtual int parse(const ReqHead& head)
    {
        is_plaintext = head.plaintext().empty()?false:true;//是否明文
        uid = head.uid();
        return uid.empty() ? ERR_UID_UNLAW : 0;
    }
    
    virtual int handle(const string& in,string& out)
    {
        EchoRedisReq reqBody;
        EchoRedisRsp rspBody;
        
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
        
        pEcho->SetValue("foo",reqBody.foo());
        
        string value;
        pEcho->GetValue("foo",value);
        rspBody.set_foo(value);
    
        if(is_plaintext)
		{
        	pb2json(&rspBody, out);
		}
		else
		{
			out = rspBody.SerializeAsString();
		}
        
        LOG(INFO)<<uid << "|" << reqBody.ShortDebugString() << "|" << rspBody.ShortDebugString();
        return 0;
    }
};

#endif

