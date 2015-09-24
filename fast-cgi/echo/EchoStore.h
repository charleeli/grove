#ifndef _ECHOSTORE_H_
#define _ECHOSTORE_H_

#include "Command.h"
#include "EchoDao.h"

using namespace pbjson;

class EchoStore : public Command
{
private:
    bool is_plaintext;//明文传输
    string uid;
    
    EchoDao* pEchoDao;
    
public:
    virtual void initialize()
    {
        Command::initialize();
        pEchoDao = new EchoDao();
    }
    
    virtual void destroy()
    {
        Command::destroy();
        if(pEchoDao)
        {
            delete pEchoDao;
            pEchoDao = NULL;
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
        EchoStoreReq reqBody;
        EchoStoreRsp rspBody;
        
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
        
        pEchoDao->SetValueToStore("foo",reqBody);
        
        EchoStoreReq req;
        pEchoDao->GetValueFromStore("foo",req);
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
