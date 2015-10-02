#ifndef _WEB_COMMON_H_
#define _WEB_COMMON_H_

// c header
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/wait.h>
#include <netdb.h>

//	stl header
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

#define FLASH_QB_NOT_ENOUGH 100

#ifndef SIZE_K
#define SIZE_K(size) ((size) * 1024)
#endif
/*
	ALIGN() 仅用power(2,n)对齐

	该宏所做的无非就是计算出最接近size的boundary的整数倍的整数。
	通常情况下size大小为整数即可，而boundary则必须保种为2的倍数。
	比如ALIGN(7,4)为8；ALIGN(21,8)为24；ALIGN(21,16)则为32。
	不过Apache中用的最多的还是ALIGN_DEFAULT，其实际上是ALIGN(size,8)。
	在以后的地方，我们将这种处理方式称之为“8对齐”或者“4K对齐”或者类似。
*/
#define ALIGN(size, boundary) (((size)+((boundary)-1))&~((boundary)-1))

//	8对齐
#define ALIGN_8(size) ALIGN(size, 8)

#define CSL_MAX(a,b) ((a)>(b)?(a):(b))
#define CSL_MIN(a,b) ((a)<(b)?(a):(b))


#ifdef WEBLIB_WITH_FASTCGI
#include "fcgi_config.h"
#include "fcgiapp.h"
#include "fcgio.h"
const int DEFAULT_MAX_SERIAL_NO = 10000;
const int NO_LIMIT_SERIAL_NO = 0;
extern   FCGX_Stream *fcgi_in, *fcgi_out, *fcgi_err;
extern   FCGX_ParamArray fcgi_env;
#endif


#endif
