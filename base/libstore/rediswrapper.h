#ifndef _REDISWRAPPER_H_
#define _REDISWRAPPER_H_

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <hiredis/hiredis.h>

using namespace std;

#define REDIS_INIT      (-10000)
#define REDIS_NODATA    (-9999)
#define REDIS_DECODE    (-9998)
#define REDIS_STORE     (-9997)
#define REDIS_EXECUTE   (-9996)

class RedisWrapper
{   
private:
    redisContext    *_pRedis;
    
public:
    RedisWrapper(): _pRedis(nullptr)
    {
    }

    RedisWrapper(const string& sHost, int port, int timeout) : _pRedis(nullptr)
    {
        initialize(sHost, port, timeout);
    }

    virtual ~RedisWrapper()
    {
        disconnect();
    }
 
    int initialize(const string& sHost = "127.0.0.1", int port = 6379, int timeout = 500)
    {      
        struct timeval tv;
        tv.tv_sec  = timeout / 1000;
        tv.tv_usec = (timeout * 1000 ) % 1000000;
        _pRedis = redisConnectWithTimeout(sHost.c_str(), port, tv);
        if (_pRedis == NULL || _pRedis->err) 
        {
            if (_pRedis)
            {
                redisFree(_pRedis);
            }
            return REDIS_INIT;
        }
        redisEnableKeepAlive(_pRedis);  
        return 0;
    }

    void disconnect()
    {
        if (_pRedis != NULL)
        {
            redisFree(_pRedis);
            _pRedis = NULL;
        }
    }

    template<class T>
    int getProto(const string& key, T& t)
    {
        redisReply *r = (redisReply *)redisCommand(_pRedis,"GET %b", key.c_str(), key.size());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }

        if (r->len == 0)
        {
            return REDIS_NODATA;
        }
        
        bool bRet = t.ParseFromArray(r->str, r->len);
        freeReplyObject(r);
        return bRet ? 0 : REDIS_DECODE;
    }

    template<class T>
    int getProto(const string& key, T* t)
    {
        redisReply *r = (redisReply *)redisCommand(_pRedis,"GET %b", key.c_str(), key.size());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }

        if (r->len == 0)
        {
            freeReplyObject(r);
            return REDIS_NODATA;
        }

        bool bRet = t->ParseFromArray(r->str, r->len);
        freeReplyObject(r);
        return bRet ? 0 : REDIS_DECODE;
    }

    template<class T>
    int setProto(const string& key, const T& t, int iExpireTime = -1)
    {
        redisReply *r = NULL;
        string v = t.SerializeAsString();
        if (iExpireTime == -1)
            r = (redisReply *)redisCommand(_pRedis, 
                                            "SET %b %b", 
                                            key.c_str(), key.size(), v.c_str(), v.size());
        else 
            r = (redisReply *)redisCommand(_pRedis, 
                                            "SET %b %b ex %d", 
                                            key.c_str(), key.size(), v.c_str(), v.size(), iExpireTime);
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
        freeReplyObject(r);
        return 0;
    }

    int del(const string& key)
    {
        redisReply *r = (redisReply *)redisCommand(_pRedis,"DEL %b", key.c_str(), key.size());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
        freeReplyObject(r);
        return 0;
    }

    template<class T>
    int getBatchProto(const vector<string>& v, map<string, T>& m, bool bPadding = false)
    {
        int iErrCode = 0;
        for (size_t index = 0; index < v.size(); index++)
        {
            redisAppendCommand(_pRedis, "GET %b", v[index].c_str(), v[index].size());
        }

        redisReply* r = NULL;
        for (size_t index = 0; index < v.size(); index++)
        {
            int iRet = (int)redisGetReply(_pRedis, (void**)&r);
            if (iRet == REDIS_OK)
            {
                T t;
                if (r->len > 0 && t.ParseFromArray(r->str, r->len))
                {
                    m[v[index]] = t;
                }
                else if (r->len == 0 && bPadding)
                {
                    m[v[index]] = t;
                }
            }
            else
            {
                iErrCode++;
            }

            if (r != NULL)
            {
                freeReplyObject(r);
            }
        }
        return iErrCode;
    }

    template<typename T>
    int setBatchProto(const map<string, T>& m, map<string, int>& mRes)
    {
        int iErrCode = 0;
		typename map<string, T>::const_iterator itm;
        for (itm = m.begin(); itm != m.end(); itm++)
        {
            string v = itm->second.SerializeAsString();
            redisAppendCommand(_pRedis, 
                                "SET %b %b", 
                                itm->first.c_str(), itm->first.size(), v.c_str(), v.size());
        }

        redisReply* r = NULL;
        for (itm = m.begin(); itm != m.end(); itm++)
        {
            int iRet = (int)redisGetReply(_pRedis, (void**)&r);
            if (r != NULL)
            {
                freeReplyObject(r);
            }
            iErrCode += (iRet == REDIS_OK) ? 0 : 1;
            mRes[itm->first] = iRet;
        }
        return iErrCode;
    }

    int delBatchProto(const vector<string>& v, map<string, int>& mRes)
    {
        int iErrCode = 0;
        for (size_t index = 0; index < v.size(); index++)
        {
            redisAppendCommand(_pRedis, "DEL %b", v[index].c_str(), v[index].size());
        }

        redisReply* r = NULL;
        for (size_t index = 0; index < v.size(); index++)
        {
            int iRet = (int)redisGetReply(_pRedis, (void**)&r);
            if (r != NULL)
            {
                freeReplyObject(r);
            }
            iErrCode += (iRet == REDIS_OK) ? 0 : 1;
            mRes[v[index]] = iRet;
        }
        return iErrCode;
    }
    
    int setValue(const string& key, const string& data, int iExpireTime = 0)
    {
       
        string sCommand = "SET";
        char strpirTime[16];
        sprintf(strpirTime, " ex %d", iExpireTime);
        if (iExpireTime == 0)
            sCommand = sCommand +" "+key+" "+data;
        else 
            sCommand = sCommand+" "+key+" "+data+" "+strpirTime;
        
        redisReply *r = (redisReply *)redisCommand(_pRedis, sCommand.c_str());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
        freeReplyObject(r);
        return 0;
    }
   
	int  getValue(const string &key, string &out)
	{
		redisReply *r = (redisReply *)redisCommand(_pRedis,"GET %b", key.c_str(), key.size());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
 
        if (r->len == 0)
        {
            freeReplyObject(r);
            return REDIS_NODATA;
        }

        out = string(r->str, r->len);
        freeReplyObject(r);
        return  0;
	}
	
    template<typename T>
    int getAllProtoFromHash(const string &hashtable,  vector<T> &v)
    {
        redisReply *r = NULL;
        r= (redisReply *)redisCommand(_pRedis,"HVALS %s", hashtable.c_str());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
 
        if (r->type == REDIS_REPLY_ARRAY) 
        {
            for (size_t  j = 0; j<r->elements; j++) 
            {
                T t;
                t.ParseFromArray(r->element[j]->str, r->element[j]->len);
                v.push_back(t);
            }
        }
        
        freeReplyObject(r);
        return v.size();
    }
    
    template<typename T>
    int getProtoFromHash(const string &hashtable, const string &key, T &v)
    {
        redisReply *r = NULL;
        r= (redisReply *)redisCommand(_pRedis,"HGET %s %s", hashtable.c_str(), key.c_str());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
 
        if (r->len == 0)
        {
             freeReplyObject(r);
            return REDIS_NODATA;
        }
        
        v.ParseFromArray(r->str, r->len);
        
        freeReplyObject(r);
        return 0;
    }
    
    template<typename T>
    int setProtoFromHash(const string &hashtable, const string &key, T &v)
    {
       
        string sCommand;
        sCommand = "HSET  "+hashtable+" "+key+" "+v.SerializeAsString();
 
        redisReply *r = (redisReply *)redisCommand(_pRedis, sCommand.c_str());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
        freeReplyObject(r);
        return 0;
    }
    
    int push2queueTail(const string &key, const string &v)
    {
        string sCommand;
        sCommand = "RPUSH  "+key+" "+v;
 
        redisReply *r = (redisReply *)redisCommand(_pRedis, sCommand.c_str());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }
        freeReplyObject(r);
        return 0;
    }
  
    bool isHashKeyExist(const string& hashtable, const string& key)
    {
        redisReply *r = (redisReply *)redisCommand(_pRedis,"HEXISTS  %s %s", hashtable.c_str(), key.c_str());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }

        bool bExist = (r->integer > 0);
        freeReplyObject(r);
        return bExist;
    }
 
    bool isKeyExist(const string& key)
    {
        redisReply *r = (redisReply *)redisCommand(_pRedis,"exists %b", key.c_str(), key.size());
        if (_pRedis->err != 0 || r == NULL)
        {
            return REDIS_EXECUTE;
        }

        bool bExist = (r->integer > 0);
        freeReplyObject(r);
        return bExist;
    }

    string getError()
    {
        if (_pRedis != NULL)
        {
            return string(_pRedis->errstr);
        }
        return "";
    }

    redisContext* getRedis()
    {
        return _pRedis;
    }

};
#endif
