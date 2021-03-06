#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <error.h>
#include <stdio.h>   
#include <cassert>
#include <time.h>
#include "GLogHelper.h"
#include "CmdHandler.h"

//包含命令处理头文件
#include "Echo.h"

//注册命令
int RegisterModuleList()
{
    FastCGI_FRAME->RegisterModule<Echo>("Echo");
    
    return 0;
}

//删除命令
int DestroyModuleList()
{
    FastCGI_FRAME->DestroyModuleList();
    return 0;
}

int main(int argc, char ** argv)
{
    //初始化日志
    GLogHelper gh("echo","/usr/local/grove/log/");
    //注册命令
    RegisterModuleList();
    LOG_INFO<<"FastCGI running!";
    //处理请求
    while (FCGI_Accept() >= 0)
    {
        string content;
        FastCGI_FRAME->runAll(content);
        printf( "Content-type:application/json\r\n\r\n" );
        printf("%s\n", content.c_str());
    }
    //删除命令
    LOG_INFO<<"FastCGI try to destroy!";
    DestroyModuleList();
    LOG_INFO<<"FastCGI destroyed!";
    return 0;
}

