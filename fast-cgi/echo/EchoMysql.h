#ifndef _ECHOMYSQL_H_
#define _ECHOMYSQL_H_

#include "Command.h"
#include "EchoWrapper.h"

using namespace pbjson;

class EchoMysql : public Command
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
        EchoMysqlReq reqBody;
        EchoMysqlRsp rspBody;
        
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
        
        pEcho->SetValueToMysql("foo",reqBody);
        
        EchoMysqlReq req;
        pEcho->GetValueFromMysql("foo",req);
        rspBody.set_foo(req.foo());
    
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

