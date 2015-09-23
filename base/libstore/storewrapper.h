#ifndef _STOREWRAPPER_H_
#define _STOREWRAPPER_H_

#include "mysqlwrapper.h"
#include "rediswrapper.h"

using google::protobuf::Message;
using namespace Grove;
using namespace std;

class StoreWrapper
{
private:
    MysqlWrapper    *pMysql_;
    RedisWrapper    *pRedis_;
    
public:
    StoreWrapper(const string& mysqlHost,
                          int  mysqlPort, 
                 const string& mysqlUser, 
                 const string& mysqlPasswd, 
                 const string& mysqlDatabase,
                 const string& mysqlTable,
                 const string& redisHost,
                           int redisPort,
                           int redisTimeout
                )
    {
        pMysql_ = new MysqlWrapper(mysqlHost,
                                    mysqlPort,
                                    mysqlUser,
                                    mysqlPasswd,
                                    mysqlDatabase,
                                    mysqlTable);
        pRedis_ = new RedisWrapper(redisHost,
                                    redisPort,
                                    redisTimeout);
    }
    
    ~StoreWrapper()
    {
        if(pMysql_)
        {
            delete pMysql_;
            pMysql_ = NULL;
        }
        
        if(pRedis_)
        {
            delete pRedis_;
            pRedis_ = NULL;
        }
    }  

    template<class T>
    int getProto(const string& key, T& t)
    {
        if(0 != pRedis_->getProto(key, t))
        {
            return pMysql_->getProto(key, t);
        }
        
        return 0;
    }
    
    template<class T>
    int getProto(const string& key, T* t)
    {
        if(0 != pRedis_->getProto(key, t))
        {
            return pMysql_->getProto(key, t);
        }
    
        return 0;
    }
    
    template<class T>
    int setProto(const string& key, const T& t)
    {
        if(0 != pMysql_->setProto(key, t))
        {
            return -1;
        }

        return pRedis_->setProto(key, t);
    }
    
    int del(const string& key)
    {
        if(0 != pMysql_->del(key))
        {
            return -1;
        }
        
        return pRedis_->del(key);
    }
    
    bool isKeyExist(const string& key)
    {
        if(!pRedis_->isKeyExist(key))
        {
            return pMysql_->isKeyExist(key);
        }
    }
};

#endif

