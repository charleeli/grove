#ifndef _ECHODAO_H_
#define _ECHODAO_H_

#include "rediswrapper.h"
#include "mysqlwrapper.h"
#include "storewrapper.h"
#include "JsonHelper.h"
#include "Echo.pb.h"

using google::protobuf::Message;
using namespace Grove;
using namespace std;

class EchoDao 
{
private:
    RedisWrapper*   pEchoRedis;
    MysqlWrapper*   pEchoMysql;
    StoreWrapper*   pEchoStore;
    
public:
    EchoDao()
    {
        JsonHelper jh("../../conf/fast-cgi/EchoModule.json");
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
        string  tablename= jh.Root["EchoMysql"]["tablename"].GetString();
        
        pEchoMysql = new MysqlWrapper(host, port,user, password, dbname,tablename);
        LOG(INFO)<<"EchoMysql host:"<<host<<" port:"<<port;
        LOG(INFO)<<"EchoMysql user:"<<user<<" password:"<<password<<" dbname:"<<dbname;
        
        pEchoStore = new StoreWrapper(host, port,user, password, dbname,tablename,
                                    host_, port_, timeout_);
        LOG(INFO)<<"Store inited";
    }

    ~EchoDao()
    {
        if (pEchoRedis) {
           delete pEchoRedis;
           pEchoRedis = NULL;
        }
        
        if (pEchoMysql) {
           delete pEchoMysql;
           pEchoMysql = NULL;
        } 
        
        if (pEchoStore) {
           delete pEchoStore;
           pEchoStore = NULL;
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
        return pEchoMysql->setProto(key, t);
    }
    
    template<class T>
    int GetValueFromMysql(const string& key, T& t)
    {
        return pEchoMysql->getProto(key, t);
    }
    
    template<class T>
    int SetValueToStore(const string& key, const T& t)
    {
        return pEchoStore->setProto(key, t);
    }
    
    template<class T>
    int GetValueFromStore(const string& key, T& t)
    {
        return pEchoStore->getProto(key, t);
    }
};

#endif

