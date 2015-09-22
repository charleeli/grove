#ifndef _MYSQLWRAPPER_H_
#define _MYSQLWRAPPER_H_

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "mysql.h"

using namespace std;

#define MYSQL_INIT      (-10000)
#define MYSQL_NODATA    (-9999)
#define MYSQL_DECODE    (-9998)
#define MYSQL_STORE     (-9997)
#define MYSQL_EXECUTE   (-9996)

class MysqlWrapper
{
private:
    MYSQL *_pstMql;
    string _host;
    string _user;
    string _password;
    string _database;
    string _table;
    string _charset;
    int _port;
    int _flag;
    bool _bConnected;
    string _sLastSql;   
public:
    MysqlWrapper(const string& sHost,
                          int  port, 
                const string& sUser, 
                const string& sPasswd, 
                const string& sDatabase ,
                const string& sTable
                ) 
    : _pstMql(nullptr), 
    _host(sHost), 
    _user(sUser), 
    _password(sPasswd), 
    _database(sDatabase),
    _table(sTable)
    {
        initialize(sHost, sUser, sPasswd, sDatabase,sTable, port);
    }

    virtual ~MysqlWrapper()
    {
        disconnect();
    }
  
    int initialize(const string& sHost, 
                    const string& sUser, 
                    const string& sPasswd,
                    const string& sDatabase, 
                    const string& sTable, 
                    int port = 3306, 
                    const string &sCharSet = "", 
                    int iFlag = 0)
    {      
        _host        = sHost;
        _user        = sUser;
        _password    = sPasswd;
        _database    = sDatabase;
        _table       = sTable,
        _charset     = sCharSet;
        _port        = port;
        _flag        = 0;
        _bConnected  = false;
        return 0;
    }

    void connect()
    {
        if (_pstMql == NULL)
        {
            _pstMql = mysql_init(NULL);
        }
      
        if(!_charset.empty()) 
        {
            if (mysql_options(_pstMql, MYSQL_SET_CHARSET_NAME, _charset.c_str())) 
            {
                // TODO
                return;
            }
        }
        else
        {
            if (mysql_options(_pstMql, MYSQL_SET_CHARSET_NAME, "utf8")) 
            {
                // TODO
                return;
            }
        }

        if (mysql_real_connect(_pstMql, 
                            _host.c_str(),  
                            _user.c_str(), 
                            _password.c_str(), 
                            _database.c_str(), 
                            _port, NULL, 
                            _flag) == NULL) 
        {
            // TODO
            return;
        }
        
        //如果没有创建_tabale表，则创建
        ostringstream osql;
        osql << "CREATE TABLE IF NOT EXISTS " << _table 
             << "("
             << "ukey varchar(64) not null primary key, "
             << "udata blob, "
             << "ts TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"
             << ")";
        _sLastSql = osql.str();
        int iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
        if(iRet != 0)
        {
            int iErrno = mysql_errno(_pstMql);
            if (iErrno == 2013 || iErrno == 2006)
            {
                connect();  
                iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
            }
        }
        if (iRet != 0)
        {
            return;
        }
        
        _bConnected = true;
    }
    
    void disconnect()
    {
        if (_pstMql != NULL)
        {
            mysql_close(_pstMql);
            _pstMql = NULL;
        }
        
        _bConnected = false;
    }
  
    string escapeString(const string& sFrom)
    {
        if(!_bConnected)
        {
            connect();
        }

        string::size_type iLen = sFrom.length() * 2 + 1;
        char *pTo = (char *)malloc(iLen);
        memset(pTo, 0x00, iLen);
        mysql_real_escape_string(_pstMql, pTo, sFrom.c_str(), sFrom.length());

        string sTo = pTo;
        free(pTo);
        return sTo;
    }
   
    template<class T>
    int getProto(const string& key, T& t)
    {
        if(!_bConnected)
        {
            connect();
        }
        
        ostringstream osql;
        osql << "SELECT udata FROM " <<  _table << " WHERE ukey='" << key << "'";
        _sLastSql = osql.str();
        int iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
        if(iRet != 0)
        {
            int iErrno = mysql_errno(_pstMql);
            if (iErrno == 2013 || iErrno == 2006)
            {
                connect();  
                iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
            }
        }
        if (iRet != 0)
        {
            return MYSQL_EXECUTE;
        }
        
        MYSQL_RES *pstRes = mysql_store_result(_pstMql);
        if(pstRes == NULL)
        {
            return MYSQL_STORE;
        }
        
        MYSQL_ROW stRow = mysql_fetch_row(pstRes);
        unsigned long * lengths = mysql_fetch_lengths(pstRes);
        if (stRow == NULL || lengths == NULL)
        {
            return MYSQL_NODATA;
        }

        iRet = t.ParseFromArray(stRow[0], lengths[0]);
        mysql_free_result(pstRes);
        return  iRet ? 0 : MYSQL_DECODE;
    }
    
    template<class T>
    int getProto(const string& key, T* t)
    {
        if(!_bConnected)
        {
            connect();
        }
        
        ostringstream osql;
        osql << "SELECT udata FROM " <<  _table << " WHERE ukey='" << key << "'";
        _sLastSql = osql.str();
        int iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
        if(iRet != 0)
        {
            int iErrno = mysql_errno(_pstMql);
            if (iErrno == 2013 || iErrno == 2006)
            {
                connect();  
                iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
            }
        }
        if (iRet != 0)
        {
            return MYSQL_EXECUTE;
        }
        
        MYSQL_RES *pstRes = mysql_store_result(_pstMql);
        if(pstRes == NULL)
        {
            return MYSQL_STORE;
        }
        
        MYSQL_ROW stRow = mysql_fetch_row(pstRes);
        unsigned long * lengths = mysql_fetch_lengths(pstRes);
        if (stRow == NULL || lengths == NULL)
        {
            return MYSQL_NODATA;
        }

        iRet = t->ParseFromArray(stRow[0], lengths[0]);
        mysql_free_result(pstRes);
        return  iRet ? 0 : MYSQL_DECODE;
    }

    template<class T>
    int setProto(const string& key, const T& t)
    {
        ostringstream osql;
        string data = escapeString(t.SerializeAsString());
        osql << "REPLACE INTO " <<  _table << "(ukey, udata) VALUE ('" << key << "', '" << data << "')";
        
        if(!_bConnected)
        {
            connect();
        }
        
        _sLastSql = osql.str();
        int iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
        if(iRet != 0)
        {
            int iErrno = mysql_errno(_pstMql);
            if (iErrno == 2013 || iErrno == 2006)
            {
                connect(); 
                iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
            }
        }

        return iRet;
    }

    int del(const string& key)
    {
        ostringstream osql;
        osql << "DELETE FROM " <<  _table << " WHERE ukey='" << key << "'";
        if(!_bConnected)
        {
            connect();
        }
        
        _sLastSql = osql.str();
        int iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
        if(iRet != 0)
        {
            int iErrno = mysql_errno(_pstMql);
            if (iErrno == 2013 || iErrno == 2006)
            {
                connect();  
                iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
            }
        }

        return iRet;
    }
    
    bool isKeyExist(const string& key)
    {
        if(!_bConnected)
        {
            connect();
        }

        ostringstream osql;
        osql << "SELECT udata FROM " <<  _table << " WHERE ukey='" << key << "'";
        _sLastSql = osql.str();
        int iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
        if(iRet != 0)
        {
            int iErrno = mysql_errno(_pstMql);
            if (iErrno == 2013 || iErrno == 2006)
            {
                connect();  
                iRet = mysql_real_query(_pstMql, _sLastSql.c_str(), _sLastSql.length());
            }
        }
        if (iRet != 0)
        {
            return false;
        }

        MYSQL_RES *pstRes = mysql_store_result(_pstMql);
        if(pstRes == NULL)
        {
            return false;
        }

        bool bExist = pstRes->row_count > 0;
        mysql_free_result(pstRes);
        return bExist;
    }

    string getError()
    {
        string sErr = "";
        if (_pstMql != NULL && mysql_error(_pstMql) != NULL)
        {
            sErr = string(mysql_error(_pstMql));
        }
        return sErr;
    }

    string getlastsql()
    {
        return _sLastSql;
    }

};
#endif
