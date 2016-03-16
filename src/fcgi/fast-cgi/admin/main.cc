#ifndef lint
static const char rcsid[] = "$Id: echo-x.c,v 1.1 2001/06/19 15:06:17 robs Exp $";
#endif /* not lint */

#include "fcgi_config.h"
#include "fcgiapp.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <error.h>
#include <stdio.h>   
#include <cassert>
#include <time.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

extern char **environ;
FCGX_Stream *fcgi_in, *fcgi_out, *fcgi_err;
FCGX_ParamArray fcgi_envp;

#include "weball.h"
#include "GLogHelper.h"
#include "CmdHandler.h"

//包含命令处理头文件
#include "Login.h"

//注册命令
int RegisterModuleList()
{
    FastCGI_FRAME->RegisterModule<Login>("Login");
    
    return 0;
}

//删除命令
int DestroyModuleList()
{
    FastCGI_FRAME->DestroyModuleList();
    return 0;
}

int main ()
{
    //初始化日志
    GLogHelper gh("admin","/usr/local/grove/log/");
    //注册命令
    RegisterModuleList();
    LOG_INFO<<"FastCGI running!";

    while (FCGX_Accept(&fcgi_in, &fcgi_out, &fcgi_err, &fcgi_envp) >= 0) {
        char *contentLength = FCGX_GetParam("CONTENT_LENGTH", fcgi_envp);
        int len = 0;

        if (contentLength != NULL)
            len = strtol(contentLength, NULL, 10);

        if (len <= 0) {
            FCGX_FPrintF(fcgi_out, "No data from standard input.<p>\n");
        }
        else {
            int i, ch;

            FCGX_FPrintF(fcgi_out, "Standard input:<br>\n<pre>\n");
            for (i = 0; i < len; i++) {
                if ((ch = FCGX_GetChar(fcgi_in)) < 0) {
                    FCGX_FPrintF(fcgi_out,
                        "Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                FCGX_PutChar(ch, fcgi_out);
            }
            FCGX_FPrintF(fcgi_out, "\n</pre><p>\n");
        }

	FastCGI_FRAME->runAll();
    } /* while */

    //删除命令
    LOG_INFO<<"FastCGI try to destroy!";
    DestroyModuleList();
    LOG_INFO<<"FastCGI destroyed!";

    return 0;
}

