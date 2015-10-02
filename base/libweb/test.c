/*
 *  g++ -o AdminModule test.c -L. -lweb -lfcgi 
 *  sudo mv AdminModule /usr/local/grove/fast-cgi/
 *  cd /usr/local/grove/fast-cgi/
 *  sudo killall AdminModule
 *  sudo spawn-fcgi -a 127.0.0.1 -p 19002 -C 1 -f /usr/local/grove/fast-cgi/AdminModule 
 */
#ifndef lint
static const char rcsid[] = "$Id: echo-x.c,v 1.1 2001/06/19 15:06:17 robs Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "fcgiapp.h"

FCGX_Stream *fcgi_in, *fcgi_out, *fcgi_err;
FCGX_ParamArray fcgi_envp;

#include "weball.h"

static void PrintEnv(FCGX_Stream *out, char *label, char **envp)
{
    FCGX_FPrintF(fcgi_out, "%s:<br>\n<pre>\n", label);
    for( ; *fcgi_envp != NULL; fcgi_envp++) {
        FCGX_FPrintF(fcgi_out, "%s\n", *fcgi_envp);
    }
    FCGX_FPrintF(fcgi_out, "</pre><p>\n");
}

int main ()
{
    int count = 0;

    while (FCGX_Accept(&fcgi_in, &fcgi_out, &fcgi_err, &fcgi_envp) >= 0) {
        char *contentLength = FCGX_GetParam("CONTENT_LENGTH", fcgi_envp);
        int len = 0;

        /*FCGX_FPrintF(fcgi_out,
           "Content-type: text/html\r\n"
           "\r\n"
           "<title>FastCGI echo (fcgiapp version)</title>"
           "<h1>FastCGI echo (fcgiapp version)</h1>\n"
           "Request number %d,  Process ID: %d<p>\n", ++count, getpid());*/

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

        //PrintEnv(fcgi_out, "Request environment", fcgi_envp);
        //PrintEnv(fcgi_out, "Initial environment", environ);

	webpage page;
	page.load("/usr/local/grove/fast-cgi/html/query_user.html");
	page.output();
    } /* while */

    return 0;
}
