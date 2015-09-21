#ifndef _ECHOWRAPPER_H_
#define _ECHOWRAPPER_H_

#include "rediswrapper.h"
#include "mysqlwrapper.h"
#include "JsonHelper.h"
#include "Echo.pb.h"

using google::protobuf::Message;
using namespace Grove;
using namespace std;

class EchoWrapper 
{
private:
    RedisWrapper*   pEchoRedis;
    MysqlWrapper*   pEchoMysql;
    
public:
    EchoWrapper()
    {
        JsonHelper jh("../dao/EchoModule.json");
        string  host_    = jh.Root["EchoRedis"]["host"].GetString();
        int     port_    = jh.Root["EchoRedis"]["port"].GetInt();
        int     timeout_ = jh.Root["EchoRedis"]["timeout"].GetInt();
  
        pEchoRedis = new RedisWrapper(host_, port_, timeout_);
        LOG(INFO)<<"EchoRedis host:"<<host_<<" port:"<<port_<<" timeout:"<<timeout_;
        
        string  host    = jh.Root["EchoMysql"]["host"].GetString();
        int     port    = jh.Root["EchoMysql"]["port"].GetInt();
        string  user    = jh.Root["EchoMysql"]["user"].GetString();
        string  password= jh.Root["EchoMysql"]["password"].GetString();
        string  dbname  = jh.Root["EchoMysql"]["dbname"].GetString();
        
        pEchoMysql = new MysqlWrapper(host, port,user, password, dbname);
        LOG(INFO)<<"EchoMysql host:"<<host<<" port:"<<port;
        LOG(INFO)<<"EchoMysql user:"<<user<<" password:"<<password<<" dbname:"<<dbname;
    }

    ~EchoWrapper()
    {
        if (pEchoRedis) {
           delete pEchoRedis;
           pEchoRedis = NULL;
        }
        
        if (pEchoMysql) {
           delete pEchoMysql;
           pEchoMysql = NULL;
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
    
    template<class T>
    int SetValueToMysql(const string& key, const T& t)
    {
        return pEchoMysql->setProto(key, t, "t_user");
    }
    
    template<class T>
    int GetValueFromMysql(const string& key, T& t)
    {
        return pEchoMysql->getProto(key, t, "t_user");
    }
};

#endif

