#ifndef _ECHOWRAPPER_H_
#define _ECHOWRAPPER_H_

#include "rediswrapper.h"
#include "JsonHelper.h"
#include "Echo.pb.h"

using google::protobuf::Message;
using namespace Grove;
using namespace std;

class EchoWrapper 
{
private:
    RedisWrapper*   pEchoRedis;
    
public:
    EchoWrapper()
    {
        JsonHelper jh("../dao/EchoModule.json");
        string  host    = jh.Root["EchoRedis"]["host"].GetString();
        int     port    = jh.Root["EchoRedis"]["port"].GetInt();
        int     timeout = jh.Root["EchoRedis"]["timeout"].GetInt();
  
        pEchoRedis = new RedisWrapper(host, port, timeout);
        LOG(INFO)<<"EchoRedis host:"<<host<<" port:"<<port<<" timeout:"<<timeout;
    }

    ~EchoWrapper()
    {
        if (pEchoRedis) {
           delete pEchoRedis;
           pEchoRedis = NULL;
        }
    }
    
    int SetValue(const string& key,const string& value)
    {
        return pEchoRedis->setValue(key,value);
    }
    
    int GetValue(const string &key, string &value)
    {
        return pEchoRedis->getValue(key,value);
    }
    
};

#endif

