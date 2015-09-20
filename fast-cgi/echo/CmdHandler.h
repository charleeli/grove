#ifndef _CMDHANDLER_H_
#define _CMDHANDLER_H_

#include <unistd.h>
#include <fcgi_stdio.h>
#include "pbjson.cpp"
#include "except_proc.h"
#include "Head.pb.h"
#include "Command.h"

using google::protobuf::Message;
using namespace pbjson;
using namespace std;

#define FastCGI_FRAME   CmdHandler::Instance()
#define FastCGI_MAX_LENGTH  (4*1024*1024)

class CmdHandler
{
public:
    //本FastCGI的命令列表
    map<string, Command*>  cmds_;  
    //接收到的信息            
    char    buffer_[FastCGI_MAX_LENGTH];        
	
private:
    //处理请求
    string handlePotocol(const string& request);
	
public:
    //由main调用
    int runAll(string& content);
    
    //返回单例
    static CmdHandler* Instance()
    {
        static CmdHandler* instance = NULL;
        if (NULL == instance)
        {
            CmdHandler* tmp = new CmdHandler;
            __sync_synchronize(); 
            instance = tmp;
        }
        
        return instance;
    }

    //销毁命令
    void DestroyModuleList() 
    {
        map<string, Command*>::iterator itm = cmds_.begin();
        for (; itm != cmds_.end(); itm++)
        {
            if (itm->second != NULL)
            {
                itm->second->destroy();
                delete itm->second;
                itm->second = NULL;
            }
        }
        cmds_.clear();
    }
    
    //注册命令
    template<class T>
    Command* RegisterModule(const string& name)
    {
        if (cmds_.find(name) == cmds_.end())
        {
            T* t = new T;
            t->initialize();
            cmds_[name] = (Command*)t;
        }
        return cmds_[name];
    }
};

//由main调用
int CmdHandler::runAll(string& content)
{
    int ret = 0;
    string msg = "";
	
    PROC_TRY_BEGIN

    //如果是GET请求
    string request = QUERY_STRING;
    //如果是POST请求
    if (string(HTTP_METHOD) == string("POST"))
    {
		//内容长度
		char *plength = getenv("CONTENT_LENGTH");
		if (plength != NULL && atoi(plength) < FastCGI_MAX_LENGTH)
		{
			//读入到buffer_
			fread(buffer_, atoi(plength), 1, stdin);  
			buffer_[atoi(plength)] = 0;
			request = string(buffer_, atoi(plength));
		}
    }

    //处理请求，讲content引用出去
    content = handlePotocol(request);
	
    PROC_TRY_END(msg, ret, -1, -1)
    return ret;
}

string CmdHandler::handlePotocol(const string& request)
{
    string response;
    
    int iRetCode = 0;
    int iErrCode = 0;
    string sErrCode = "";
    
    //请求数据
    ReqHead  req;
    //回复数据
    RspHead  rsp;

    PROC_TRY_BEGIN
	
	//将request json转化为pb
    string err;
    json2pb(request, &req, err);
    
    //判断指令是否注册
    if (cmds_.find(req.command()) == cmds_.end())
    {
        iRetCode = -1;
        sErrCode = req.command();
        PROC_TRY_EXIT(iRetCode, ERR_NO_COMMAND, iErrCode, ERR_NO_COMMAND, sErrCode, "no command")
    }

    //判断请求数据是否合法
    iErrCode = cmds_[req.command()]->parse(req);
    if (iErrCode != 0)
    {
        PROC_TRY_EXIT(iRetCode, iErrCode, iErrCode, iErrCode, sErrCode, "parse fail")
    }

    //处理请求，输入输出都是序列化后的PB字符串
    string out;
	if(!req.plaintext().empty())
	{
		iErrCode = cmds_[req.command()]->handle(req.plaintext(), out);
		rsp.set_plaintext(out);
	}
	else
	{
    	iErrCode = cmds_[req.command()]->handle(req.ciphertext(), out);
		rsp.set_ciphertext(out);
	}

    if (iErrCode != 0)
    {
        PROC_TRY_EXIT(iRetCode, iErrCode, iErrCode, iErrCode, sErrCode, "handle fail")
    }
 
    PROC_TRY_END(sErrCode, iRetCode, ERR_EXCEPTION, ERR_EXCEPTION)

    rsp.set_errcode(iRetCode);
    rsp.set_command(req.command());
    
    //将response pb转化为json
    pb2json(&rsp, response);
    if (iRetCode != 0)
    {
        LOG(ERROR)<<iRetCode << "|" << iErrCode << "|" << sErrCode << "|" << request << "|" << response;
    }

    return response;
}

#endif

